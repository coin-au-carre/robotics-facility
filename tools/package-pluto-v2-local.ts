import { createHash } from 'node:crypto'
import { lstat, mkdir, readdir, readFile, writeFile } from 'node:fs/promises'
import path from 'node:path'
import { fileURLToPath, pathToFileURL } from 'node:url'
import { type ArchiveEntry, makeArchive } from './package-archive.ts'

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..')
const hash = (bytes: Buffer) => createHash('sha256').update(bytes).digest('hex')

interface Inputs {
  schemaVersion: number
  version: string
  companionSha256: string
  assets: Record<string, string>
  sourceInventory: Array<{ path: string; sha256: string }>
}

async function regularFile(directory: string, name: string): Promise<Buffer> {
  if (!/^[A-Za-z0-9_.-]+(?:\/[A-Za-z0-9_.-]+)*$/.test(name))
    throw new Error(`Invalid package input path: ${name}`)
  let current = path.resolve(directory)
  if (!(await lstat(current)).isDirectory()) throw new Error('Input directory must be regular')
  const parts = name.split('/')
  for (const [index, part] of parts.entries()) {
    current = path.join(current, part)
    const stat = await lstat(current)
    if (stat.isSymbolicLink() || (index < parts.length - 1 ? !stat.isDirectory() : !stat.isFile()))
      throw new Error(`Package input must not be linked: ${name}`)
  }
  return readFile(current)
}

async function verified(directory: string, name: string, digest: string): Promise<Buffer> {
  const bytes = await regularFile(directory, name)
  if (!/^[a-f0-9]{64}$/.test(digest) || hash(bytes) !== digest)
    throw new Error(`Pinned Pluto input mismatch: ${name}`)
  return bytes
}

/** Assemble already-reviewed user-supplied assets, preserving the exact companion artifact. */
export async function packagePlutoLocal(bundleDirectory: string, outputName: string) {
  if (!/^[a-z0-9][a-z0-9-]*$/.test(outputName)) throw new Error('Invalid output name')
  const inputsBytes = await readFile(path.join(root, 'native/pluto-v2-inputs.json'))
  const inputs = JSON.parse(inputsBytes.toString('utf8')) as Inputs
  if (inputs.schemaVersion !== 1 || inputs.sourceInventory.length === 0)
    throw new Error('Invalid companion inventory')
  for (const name of ['bwapi-data/read', 'bwapi-data/write']) {
    try {
      const directory = path.join(bundleDirectory, name)
      if (!(await lstat(directory)).isDirectory() || (await readdir(directory)).length)
        throw new Error('Use a clean Pluto baseline without learning')
    } catch (error) {
      if (!(error instanceof Error && 'code' in error && error.code === 'ENOENT')) throw error
    }
  }
  const entries: ArchiveEntry[] = []
  for (const [name, digest] of Object.entries(inputs.assets))
    entries.push([`bundle/${name}`, await verified(bundleDirectory, name, digest)])
  entries.push([
    'bundle/pluto-companion.dll',
    await verified(bundleDirectory, 'pluto-companion.dll', inputs.companionSha256),
  ])
  for (const name of [
    'README.txt',
    'THIRD_PARTY.md',
    'BWAPI-LICENSE.txt',
    'COPYING.GPL-3.0.txt',
    'SB-EXPERIMENT.txt',
  ])
    entries.push([`bundle/${name}`, await regularFile(bundleDirectory, name)])
  for (const item of inputs.sourceInventory)
    entries.push([
      `source/${item.path}`,
      await verified(path.join(root, 'native/pluto-v2-companion'), item.path, item.sha256),
    ])
  entries.push(['source/inputs.json', inputsBytes])
  const descriptor = {
    name: 'Pluto v2',
    version: inputs.version,
    workingDirectory: '.',
    args: [],
    runtime: { kind: 'native' },
    races: ['terran', 'protoss', 'zerg'],
    pluto: { companionSha256: inputs.companionSha256 },
  }
  entries.push(['bundle/sb-bot.json', Buffer.from(JSON.stringify(descriptor, null, 2) + '\n')])
  const guide = await readFile(path.join(root, 'docs/pluto-v2-local.md'))
  entries.push(['START-HERE.md', guide])
  entries.push(
    ['bundle/bwapi-data/read/', Buffer.alloc(0)],
    ['bundle/bwapi-data/write/', Buffer.alloc(0)],
  )
  const info = {
    schemaVersion: 1,
    kind: 'user-supplied-local-build',
    name: 'Pluto v2',
    version: inputs.version,
    companionSha256: inputs.companionSha256,
    assets: inputs.assets,
    sourceFiles: inputs.sourceInventory.length,
    recipeSha256: hash(await readFile(fileURLToPath(import.meta.url))),
    inputsSha256: hash(inputsBytes),
    catalogApproved: false,
    files: entries.map(([name, bytes]) => ({ path: name, sha256: hash(bytes) })),
  }
  entries.push(['package-info.json', Buffer.from(JSON.stringify(info, null, 2) + '\n')])
  const output = path.join(root, 'dist/local', outputName)
  await mkdir(path.dirname(output), { recursive: true })
  await mkdir(output)
  for (const [name, bytes] of entries) {
    const destination = path.join(output, name)
    if (name.endsWith('/')) await mkdir(destination, { recursive: true })
    else {
      await mkdir(path.dirname(destination), { recursive: true })
      await writeFile(destination, bytes, { flag: 'wx' })
    }
  }
  const archive = await makeArchive(entries)
  const archivePath = path.join(output, `${outputName}.zip`)
  await writeFile(archivePath, archive, { flag: 'wx' })
  await writeFile(path.join(output, 'archive.sha256'), `${hash(archive)}  ${outputName}.zip\n`)
  return { output, archivePath, archiveSha256: hash(archive), descriptor }
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  const [bundle, output] = process.argv.slice(2)
  if (!bundle || !output)
    throw new Error('Usage: node tools/package-pluto-v2-local.ts <bundle> <name>')
  console.log(JSON.stringify(await packagePlutoLocal(path.resolve(bundle), output), null, 2))
}
