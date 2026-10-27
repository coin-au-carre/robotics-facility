import { execFileSync } from 'node:child_process'
import { createHash } from 'node:crypto'
import { mkdir, readFile, writeFile } from 'node:fs/promises'
import path from 'node:path'
import { fileURLToPath } from 'node:url'
import { downloadPlutoArchive, extractPlutoArchive, readPlutoRelease } from './fetch-pluto-v2.ts'
import { type BuiltPlutoCompanion, packagePlutoLocal } from './package-pluto-v2-local.ts'

const root = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..')
const sha = (bytes: Buffer) => createHash('sha256').update(bytes).digest('hex')
const [name] = process.argv.slice(2)
if (!name || !/^[a-z0-9][a-z0-9-]*$/.test(name))
  throw new Error('Usage: node tools/prepare-pluto-v2-local.ts <fresh-output-name>')
if (process.platform !== 'win32') throw new Error('Companion build requires Windows MSVC Win32')

const inputsBytes = await readFile(path.join(root, 'native/pluto-v2-inputs.json'))
const inputs = JSON.parse(inputsBytes.toString('utf8')) as {
  assets: Record<string, string>
  sourceInventory: Array<{ path: string; sha256: string }>
}
async function checkSource() {
  for (const item of inputs.sourceInventory) {
    if (
      sha(await readFile(path.join(root, 'native/pluto-v2-companion', item.path))) !== item.sha256
    )
      throw new Error(`Pinned companion source changed: ${item.path}`)
  }
}
await checkSource()
const pin = await readPlutoRelease()
for (const [file, digest] of Object.entries(inputs.assets)) {
  if (pin.files[file]?.sha256 !== digest) throw new Error('Official/model input pins disagree')
}
const vswhere = path.join(
  process.env['ProgramFiles(x86)'] ?? '',
  'Microsoft Visual Studio/Installer/vswhere.exe',
)
const vs = execFileSync(
  vswhere,
  [
    '-latest',
    '-products',
    '*',
    '-requires',
    'Microsoft.VisualStudio.Component.VC.Tools.x86.x64',
    '-property',
    'installationPath',
  ],
  { encoding: 'utf8' },
).trim()
if (!vs) throw new Error('Visual Studio C++ tools required')
const cmake = path.join(vs, 'Common7/IDE/CommonExtensions/Microsoft/CMake/CMake/bin/cmake.exe')
const build = path.join(root, '.build', `${name}-prepare`)
await mkdir(path.dirname(build), { recursive: true })
await mkdir(build)
const recipes = Object.fromEntries(
  await Promise.all(
    [
      'tools/prepare-pluto-v2-local.ts',
      'tools/fetch-pluto-v2.ts',
      'tools/package-pluto-v2-local.ts',
      'native/pluto-v2-upstream.json',
      'native/pluto-v2-modifications.txt',
    ].map(async file => [file, sha(await readFile(path.join(root, file)))]),
  ),
)
let last = 0
const archive = await downloadPlutoArchive(pin, path.join(root, '.build/downloads'), {
  progress: bytes => {
    if (bytes - last >= 32 * 1024 * 1024) {
      last = bytes
      console.log(`Official Pluto download: ${Math.floor((bytes / pin.sizeBytes) * 100)}%`)
    }
  },
})
const original = path.join(build, 'official')
await extractPlutoArchive(archive, original, pin)
console.log('Official ZIP and all extracted files verified')
const configure = [
  '-S',
  path.join(root, 'native/pluto-v2-companion'),
  '-B',
  path.join(build, 'cmake'),
  '-G',
  'Visual Studio 17 2022',
  '-A',
  'Win32',
]
execFileSync(cmake, configure, { cwd: root, stdio: 'inherit' })
execFileSync(
  cmake,
  [
    '--build',
    path.join(build, 'cmake'),
    '--config',
    'Release',
    '--target',
    'pluto-companion',
    '--parallel',
    '4',
  ],
  { cwd: root, stdio: 'inherit' },
)
await checkSource()
const companionPath = path.join(build, 'cmake/Release/pluto-companion.dll')
const companion: BuiltPlutoCompanion = {
  path: companionPath,
  sha256: sha(await readFile(companionPath)),
  inputsSha256: sha(inputsBytes),
  recipes,
}
const upstream = { url: pin.url, sha256: pin.sha256, tag: pin.tag }
await writeFile(
  path.join(build, 'build-info.json'),
  JSON.stringify(
    { companion, upstream, sourceInventory: inputs.sourceInventory, configure },
    null,
    2,
  ) + '\n',
  { flag: 'wx' },
)
const result = await packagePlutoLocal(original, name, { companion, upstream })
console.log(JSON.stringify(result, null, 2))
