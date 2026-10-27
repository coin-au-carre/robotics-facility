import assert from 'node:assert/strict'
import { readFile } from 'node:fs/promises'
import test from 'node:test'
import type { Catalog } from './metadata.ts'
import { successorPackage } from './repackage-purplewave-metadata.ts'

const catalog: Catalog = JSON.parse(
  await readFile(new URL('../catalog/catalog.json', import.meta.url), 'utf8'),
)
const fixture = () => {
  const pkg = structuredClone(
    catalog.bots.find(bot => bot.bot.id === 'purplewave')!.releases[0].package,
  )
  pkg.releaseId = 'purplewave-sb-6'
  pkg.version = '2026.09.22-sb.6'
  pkg.modifications = [
    { modifier: 'ShieldBattery', date: '2026-10-25', summary: 'Base isolation', scope: 'bot' },
    { modifier: 'Old attribution', date: '2026-10-25', summary: 'Upstream update', scope: 'bot' },
  ]
  return pkg
}
await test('PurpleWave metadata successor changes only three descriptor fields without mutating parent', () => {
  const parent = fixture(),
    before = structuredClone(parent),
    result = successorPackage(parent)
  const expected = structuredClone(parent)
  expected.releaseId = 'purplewave-sb-7'
  expected.version = '2026.09.22-sb.7'
  expected.modifications![1].modifier = 'ShieldBattery'
  assert.deepEqual(result, expected)
  assert.deepEqual(parent, before)
})
await test('PurpleWave metadata successor refuses another identity, release, version or missing attribution', () => {
  const parent = fixture()
  for (const input of [
    { ...parent, botId: 'other' },
    { ...parent, releaseId: 'purplewave-sb-4' },
    { ...parent, version: 'other' },
    { ...parent, modifications: [] },
  ])
    assert.throws(() => successorPackage(input))
})

await test('PurpleWave repack refuses a parent archive with changed bytes before assembly', async t => {
  const { mkdtemp, writeFile, rm } = await import('node:fs/promises')
  const { tmpdir } = await import('node:os')
  const { join } = await import('node:path')
  const { repackagePurpleWaveMetadata } = await import('./repackage-purplewave-metadata.ts')
  const directory = await mkdtemp(join(tmpdir(), 'purplewave-parent-'))
  t.after(() => rm(directory, { recursive: true, force: true }))
  const file = join(directory, 'changed.zip')
  await writeFile(file, 'invalid parent')
  await assert.rejects(repackagePurpleWaveMetadata(file), /Parent archive size changed/)
})
