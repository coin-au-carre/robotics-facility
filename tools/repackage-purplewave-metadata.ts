import assert from 'node:assert/strict'
import { execFileSync } from 'node:child_process'
import { readFile } from 'node:fs/promises'
import path from 'node:path'
import { fileURLToPath, pathToFileURL } from 'node:url'
import yauzl from 'yauzl'
import type { Package } from './metadata.ts'
import { type ArchiveEntry, recipeFiles } from './package-archive.ts'
import { sha256, verifyArchive } from './publication-archive.ts'
import { writeReleasePackage } from './release-package.ts'
import { validate } from './validate.ts'

const root = fileURLToPath(new URL('../', import.meta.url))
export const parentArtifact = {
  url: 'https://github.com/ShieldBattery/robotics-facility/releases/download/purplewave-sb-6/purplewave-sb-6.zip',
  sha256: '60f80501b9d898b0b392df624e35d15df4bcb3749338e77b8e78820405e980b4',
  sizeBytes: 20065845,
  manifestSha256: '18d363c412fad4c50a7d3d4c7cb52dc9f4219b76ef80259cb66f3b73bba6b21a',
  format: 'zip' as const,
}

/** Changes only release identity and downstream project attribution in the reviewed descriptor. */
export function successorPackage(parent: Package): Package {
  if (
    parent.botId !== 'purplewave' ||
    parent.releaseId !== 'purplewave-sb-6' ||
    parent.version !== '2026.09.22-sb.6'
  )
    throw new Error('Expected the reviewed PurpleWave sb.6 package')
  if (parent.modifications?.[1]?.scope !== 'bot') throw new Error('Missing bot modification record')
  const pkg = structuredClone(parent)
  pkg.releaseId = 'purplewave-sb-7'
  pkg.version = '2026.09.22-sb.7'
  pkg.modifications![1].modifier = 'ShieldBattery'
  return pkg
}

async function archiveEntries(bytes: Buffer): Promise<ArchiveEntry[]> {
  return new Promise((resolve, reject) => {
    yauzl.fromBuffer(
      bytes,
      { lazyEntries: true, strictFileNames: true, validateEntrySizes: true },
      (error, zip) => {
        if (error) return reject(error)
        const entries: ArchiveEntry[] = []
        zip.on('error', reject)
        zip.on('end', () => resolve(entries))
        zip.on('entry', entry => {
          if (entry.uncompressedSize > 512 * 1024 * 1024) {
            zip.close()
            reject(new Error('Archive member exceeds limit'))
            return
          }
          if (entry.fileName.endsWith('/')) {
            entries.push([entry.fileName, Buffer.alloc(0)])
            zip.readEntry()
            return
          }
          zip.openReadStream(entry, (err, stream) => {
            if (err) return reject(err)
            const chunks: Buffer[] = []
            stream.on('error', reject)
            stream.on('data', (chunk: Buffer) => chunks.push(chunk))
            stream.on('end', () => {
              entries.push([entry.fileName, Buffer.concat(chunks)])
              zip.readEntry()
            })
          })
        })
        zip.readEntry()
      },
    )
  })
}

export async function repackagePurpleWaveMetadata(input: string) {
  const bytes = await readFile(input)
  assert.equal(bytes.length, parentArtifact.sizeBytes, 'Parent archive size changed')
  assert.equal(sha256(bytes), parentArtifact.sha256, 'Parent archive digest changed')
  const entries = await archiveEntries(bytes)
  const manifest = entries.find(([name]) => name === 'package.json')?.[1]
  assert.ok(manifest, 'Missing parent descriptor')
  const parent: unknown = JSON.parse(manifest.toString('utf8'))
  validate('package', parent)
  await verifyArchive(bytes, { package: parent, artifact: parentArtifact })
  const pkg = successorPackage(parent)
  const candidate: unknown = JSON.parse(
    await readFile(path.join(root, 'bots/purplewave/bot.json'), 'utf8'),
  )
  validate('candidate', candidate)
  const revision = execFileSync('git', ['-C', root, 'rev-parse', 'HEAD'], {
    encoding: 'utf8',
  }).trim()
  const recipePath = 'tools/repackage-purplewave-metadata.ts'
  const recipe = await readFile(path.join(root, recipePath))
  const recipeHash = sha256(
    Buffer.concat([Buffer.from(recipePath + '\0'), recipe, Buffer.from('\0')]),
  )
  const provenance = await recipeFiles({ root, revision, sha256: recipeHash, paths: [recipePath] })
  const info = {
    parentArtifact,
    recipeRevision: revision,
    recipeSha256: sha256(recipe),
    changedDescriptorFields: ['releaseId', 'version', 'modifications[1].modifier'],
    preservedParentMembers: entries.length - 1,
    scope:
      'Metadata-only successor; compiled bot, dependencies, configuration, source, build record and notices unchanged',
  }
  return writeReleasePackage({
    root,
    candidate,
    pkg,
    entries: [
      ...entries.filter(([name]) => name !== 'package.json'),
      ...provenance,
      ['source/repackage-purplewave-sb-7.json', Buffer.from(JSON.stringify(info, null, 2) + '\n')],
    ],
    review: false,
  })
}

if (process.argv[1] && import.meta.url === pathToFileURL(path.resolve(process.argv[1])).href) {
  const [input, ...rest] = process.argv.slice(2)
  if (!input || rest.length)
    throw new Error('Usage: node tools/repackage-purplewave-metadata.ts <reviewed-sb6-zip>')
  repackagePurpleWaveMetadata(input)
    .then(console.log)
    .catch(error => {
      console.error(error)
      process.exitCode = 1
    })
}
