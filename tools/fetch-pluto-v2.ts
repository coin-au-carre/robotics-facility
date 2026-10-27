import { createHash, randomUUID } from 'node:crypto'
import { createReadStream, createWriteStream } from 'node:fs'
import { lstat, mkdir, readFile, rename, rm } from 'node:fs/promises'
import path from 'node:path'
import { Readable, Transform } from 'node:stream'
import { pipeline } from 'node:stream/promises'
import { fileURLToPath, pathToFileURL } from 'node:url'
import yauzl from 'yauzl'
import { archivePath } from './publication-archive.ts'

export interface PinnedFile {
  sha256: string
  sizeBytes: number
}

export interface PlutoRelease extends PinnedFile {
  tag: string
  releasePage: string
  url: string
  files: Record<string, PinnedFile>
}

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..')

export async function readPlutoRelease(): Promise<PlutoRelease> {
  return JSON.parse(await readFile(path.join(root, 'native/pluto-v2-upstream.json'), 'utf8'))
}

export async function verifyPinnedFile(file: string, pin: PinnedFile): Promise<void> {
  const stat = await lstat(file)
  if (!stat.isFile() || stat.size !== pin.sizeBytes) throw new Error('Pinned file size mismatch')
  const hash = createHash('sha256')
  for await (const chunk of createReadStream(file)) hash.update(chunk)
  if (hash.digest('hex') !== pin.sha256) throw new Error('Pinned file SHA-256 mismatch')
}

export async function downloadPlutoArchive(
  pin: PlutoRelease,
  cacheDirectory: string,
  options: { fetch?: typeof fetch; progress?: (bytes: number) => void } = {},
): Promise<string> {
  if (new URL(pin.url).protocol !== 'https:' || !/^[a-f0-9]{64}$/.test(pin.sha256))
    throw new Error('Invalid upstream pin')
  await mkdir(cacheDirectory, { recursive: true })
  if (!(await lstat(cacheDirectory)).isDirectory()) throw new Error('Cache must be a directory')
  const cached = path.join(cacheDirectory, `${pin.sha256}.zip`)
  try {
    await lstat(cached)
    await verifyPinnedFile(cached, pin)
    return cached
  } catch (error) {
    if (!(error instanceof Error && 'code' in error && error.code === 'ENOENT')) throw error
  }
  const partial = path.join(cacheDirectory, `${pin.sha256}.${randomUUID()}.part`)
  try {
    const response = await (options.fetch ?? fetch)(pin.url, {
      signal: AbortSignal.timeout(10 * 60 * 1000),
      headers: { 'User-Agent': 'ShieldBattery-robotics-facility' },
    })
    if (!response.ok || !response.body)
      throw new Error(`Official download failed: HTTP ${response.status}`)
    const declared = response.headers.get('content-length')
    if (declared !== null && Number(declared) !== pin.sizeBytes)
      throw new Error('Official download size mismatch')
    let bytes = 0
    const hash = createHash('sha256')
    const verify = new Transform({
      transform(chunk: Buffer, _encoding, done) {
        bytes += chunk.length
        if (bytes > pin.sizeBytes) return done(new Error('Official download exceeds pinned size'))
        hash.update(chunk)
        options.progress?.(bytes)
        done(null, chunk)
      },
    })
    const reader = response.body.getReader()
    const chunks = async function* () {
      try {
        for (;;) {
          const result = await reader.read()
          if (result.done) break
          yield result.value
        }
      } finally {
        reader.releaseLock()
      }
    }
    await pipeline(Readable.from(chunks()), verify, createWriteStream(partial, { flags: 'wx' }))
    if (bytes !== pin.sizeBytes || hash.digest('hex') !== pin.sha256)
      throw new Error('Official archive size or SHA-256 mismatch')
    await rename(partial, cached)
    return cached
  } finally {
    await rm(partial, { force: true })
  }
}

export async function extractPlutoArchive(
  archive: string,
  destination: string,
  pin: PlutoRelease,
): Promise<void> {
  await verifyPinnedFile(archive, pin)
  await mkdir(destination)
  try {
    await new Promise<void>((resolve, reject) => {
      yauzl.open(
        archive,
        { lazyEntries: true, strictFileNames: true, validateEntrySizes: true },
        (err, zip) => {
          if (err || !zip) return reject(err ?? new Error('Could not open official ZIP'))
          const seen = new Set<string>()
          const files = new Set<string>()
          let failed = false
          const fail = (error: unknown) => {
            if (failed) return
            failed = true
            zip.close()
            reject(error instanceof Error ? error : new Error(String(error)))
          }
          zip.on('error', fail)
          zip.on('end', () => {
            if (files.size !== Object.keys(pin.files).length)
              fail(new Error('Official ZIP is missing files'))
            else resolve()
          })
          zip.on('entry', entry => {
            ;(async () => {
              const normalized = archivePath(entry.fileName)
              if (seen.has(normalized)) throw new Error('Duplicate official ZIP entry')
              seen.add(normalized)
              const directory = entry.fileName.endsWith('/')
              const mode = (entry.externalFileAttributes >>> 16) & 0xf000
              if (
                entry.generalPurposeBitFlag & 1 ||
                (mode && mode !== (directory ? 0x4000 : 0x8000))
              )
                throw new Error('Unsupported official ZIP entry')
              if (directory) {
                if (entry.fileName !== 'pluto/' || entry.uncompressedSize !== 0)
                  throw new Error('Unexpected official ZIP directory')
                zip.readEntry()
                return
              }
              const expected = pin.files[entry.fileName]
              if (!expected || entry.uncompressedSize !== expected.sizeBytes)
                throw new Error('Unexpected official ZIP file or size')
              const output = path.join(destination, entry.fileName)
              await mkdir(path.dirname(output), { recursive: true })
              const stream = await new Promise<NodeJS.ReadableStream>((res, rej) => {
                zip.openReadStream(entry, (error, value) => {
                  if (error || !value) rej(error ?? new Error('Could not read official ZIP entry'))
                  else res(value)
                })
              })
              await pipeline(stream, createWriteStream(output, { flags: 'wx' }))
              await verifyPinnedFile(output, expected)
              files.add(entry.fileName)
              zip.readEntry()
            })().catch(fail)
          })
          zip.readEntry()
        },
      )
    })
  } catch (error) {
    await rm(destination, { recursive: true, force: true })
    throw error
  }
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  const [destination] = process.argv.slice(2)
  if (!destination) throw new Error('Usage: node tools/fetch-pluto-v2.ts <fresh-directory>')
  const pin = await readPlutoRelease()
  let last = 0
  const archive = await downloadPlutoArchive(pin, path.join(root, '.build/downloads'), {
    progress: bytes => {
      if (bytes - last >= 32 * 1024 * 1024) {
        last = bytes
        console.log(`Official Pluto download: ${Math.floor((bytes / pin.sizeBytes) * 100)}%`)
      }
    },
  })
  await extractPlutoArchive(archive, path.resolve(destination), pin)
  console.log(`Verified official Pluto ${pin.tag}: ${path.resolve(destination)}`)
}
