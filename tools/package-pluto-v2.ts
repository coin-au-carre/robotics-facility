import { execFileSync } from 'node:child_process'
import { createHash } from 'node:crypto'
import { readFile } from 'node:fs/promises'
import path from 'node:path'
import { fileURLToPath } from 'node:url'
import type { Candidate, Package, SourceLock } from './metadata.ts'
import type { ArchiveEntry } from './package-archive.ts'
import { writeReleasePackage } from './release-package.ts'
import { validate } from './validate.ts'

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..')
const [buildArg, releaseId, reviewArg] = process.argv.slice(2)
if (
  !buildArg ||
  !releaseId ||
  !/^[a-z][a-z0-9-]*$/.test(releaseId) ||
  (reviewArg && reviewArg !== '--review')
)
  throw new Error('Usage: node tools/package-pluto-v2.ts <prepared-build> <release-id> [--review]')
const sha = (bytes: Buffer) => createHash('sha256').update(bytes).digest('hex')
const buildDirectory = path.resolve(buildArg)
const build = JSON.parse(await readFile(path.join(buildDirectory, 'build-info.json'), 'utf8')) as {
  companion: { path: string; sha256: string; inputsSha256: string }
  sourceInventory: Array<{ path: string; sha256: string }>
}
const inputs = await readFile(path.join(root, 'native/pluto-v2-inputs.json'))
if (sha(inputs) !== build.companion.inputsSha256) throw new Error('Companion inputs changed')
const companion = await readFile(build.companion.path)
if (sha(companion) !== build.companion.sha256) throw new Error('Built companion digest changed')
const candidate = JSON.parse(
  await readFile(path.join(root, 'bots/pluto/bot.json'), 'utf8'),
) as Candidate
validate('candidate', candidate)
const lock = JSON.parse(await readFile(path.join(root, 'source-lock.json'), 'utf8')) as SourceLock
const sources = lock.sources.filter(s => candidate.sourceIds.includes(s.id))
const adapterSource = sources.find(s => s.id === 'pluto-companion')
if (!adapterSource || sources.length !== candidate.sourceIds.length)
  throw new Error('Missing pinned sources')
const entries: ArchiveEntry[] = [['work/pluto-companion.dll', companion]]
for (const item of build.sourceInventory) {
  const bytes = await readFile(path.join(root, 'native/pluto-v2-companion', item.path))
  if (sha(bytes) !== item.sha256) throw new Error(`Compiled source changed: ${item.path}`)
  const pinned = execFileSync(
    'git',
    ['show', `${adapterSource.revision}:native/pluto-v2-companion/${item.path}`],
    { cwd: root },
  )
  if (sha(pinned) !== item.sha256) throw new Error('Build source differs from source pin')
  entries.push([`source/companion/${item.path}`, bytes])
}
for (const [destination, source] of [
  ['pluto-upstream.json', 'native/pluto-v2-upstream.json'],
  ['notices/Companion-MIT.txt', 'native/LICENSE'],
  ['notices/BWAPI-LGPL.txt', 'native/pluto-v2-companion/vendor/bwapi/LICENSE'],
  ['notices/GPL-3.0.txt', 'native/pluto-v2-companion/vendor/bwapi/COPYING.GPL-3.0.txt'],
  ['notices/MODIFICATIONS.txt', 'native/pluto-v2-modifications.txt'],
  ['notices/PLUTO.txt', 'bots/pluto/RELEASE.txt'],
  ['source/BUILD.md', 'bots/pluto/BUILD.md'],
])
  entries.push([destination, await readFile(path.join(root, source))])
entries.push(
  ['work/bwapi-data/read/', Buffer.alloc(0)],
  ['work/bwapi-data/write/', Buffer.alloc(0)],
)
const revision = execFileSync('git', ['rev-parse', 'HEAD'], { cwd: root, encoding: 'utf8' }).trim()
const pkg: Package = {
  schemaVersion: 1,
  botId: 'pluto',
  releaseId,
  version: '2.0-md09b_1331000-sb.1',
  platform: { os: 'windows', architecture: 'x86' },
  runtime: { kind: 'pluto-v2', companionSha256: build.companion.sha256 },
  launch: { entrypoint: 'work/pluto-companion.dll', workingDirectory: 'work', arguments: [] },
  profile: candidate.profile,
  bwapi: { version: '4.4.0', protocol: 10003, minimumBridgeVersion: 'pluto-v2-catalog-1' },
  sources,
  licenses: [
    { name: 'MIT - compatibility companion', noticePath: 'notices/Companion-MIT.txt' },
    { name: 'LGPL-3.0 - BWAPI', noticePath: 'notices/BWAPI-LGPL.txt' },
    { name: 'GPL-3.0 license text', noticePath: 'notices/GPL-3.0.txt' },
    { name: 'Original Pluto download disclosure', noticePath: 'notices/PLUTO.txt' },
    { name: 'Adapter modifications', noticePath: 'notices/MODIFICATIONS.txt' },
  ],
  permissions: candidate.permissions,
  modifications: [
    {
      modifier: 'ShieldBattery',
      date: '2026-10-27',
      scope: 'dependency',
      summary:
        'Adds .10 compatibility companion with filtered legacy view, command translation and isolated lifecycle; original Pluto binaries downloaded unchanged from pinned official release.',
    },
  ],
  writableDirectories: ['work/bwapi-data/read', 'work/bwapi-data/write'],
  build: {
    recipeSource: {
      id: 'robotics-facility',
      repository: 'https://github.com/coin-au-carre/robotics-facility.git',
      revision,
      patches: [],
    },
    recipePath: 'tools/package-pluto-v2.ts',
    toolchain:
      'MSVC 19.35 Win32; CMake; exact companion source inventory verified against source pin',
  },
  sourceReview: candidate.sourceReview,
}
console.log(
  JSON.stringify(
    await writeReleasePackage({ root, candidate, pkg, entries, review: reviewArg === '--review' }),
    null,
    2,
  ),
)
