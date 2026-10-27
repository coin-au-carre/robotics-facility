import assert from 'node:assert/strict'
import { createHash } from 'node:crypto'
import { mkdtemp, readdir, readFile, rm, writeFile } from 'node:fs/promises'
import os from 'node:os'
import path from 'node:path'
import test from 'node:test'
import {
  downloadPlutoArchive,
  extractPlutoArchive,
  type PlutoRelease,
  readPlutoRelease,
} from './fetch-pluto-v2.ts'
import { type ArchiveEntry, makeArchive } from './package-archive.ts'

const hash = (bytes: Uint8Array) => createHash('sha256').update(bytes).digest('hex')
const entries: ArchiveEntry[] = [
  ['README.txt', Buffer.from('fixture release')],
  ['pluto.dll', Buffer.from('fixture model')],
  ['pluto/pluto_infer.exe', Buffer.from('fixture engine')],
  ['pluto/pluto_weights.bin', Buffer.from('fixture weights')],
]
async function fixture() {
  const directory = await mkdtemp(path.join(os.tmpdir(), 'pluto-download-'))
  const bytes = await makeArchive(entries)
  const pin: PlutoRelease = {
    tag: 'test',
    releasePage: 'https://example.invalid/release',
    url: 'https://example.invalid/pluto.zip',
    sha256: hash(bytes),
    sizeBytes: bytes.length,
    files: Object.fromEntries(
      entries.map(([name, value]) => [name, { sha256: hash(value), sizeBytes: value.length }]),
    ),
  }
  return { directory, bytes, pin }
}

await test('official model pins agree with companion input pins', async () => {
  const upstream = await readPlutoRelease()
  const inputs = JSON.parse(
    await readFile(new URL('../native/pluto-v2-inputs.json', import.meta.url), 'utf8'),
  ) as { assets: Record<string, string> }
  for (const [name, digest] of Object.entries(inputs.assets))
    assert.equal(upstream.files[name].sha256, digest)
  assert.match(upstream.url, /^https:\/\/github\.com\/tscmoo\/pluto\/releases\/download\/v2\.0\//)
})

await test('download, verified extraction and cache reuse preserve all official layout files', async () => {
  const f = await fixture()
  try {
    let calls = 0
    const request: typeof fetch = async input => {
      calls++
      assert.equal(input, f.pin.url)
      return new Response(new Uint8Array(f.bytes), {
        headers: { 'content-length': String(f.bytes.length) },
      })
    }
    const archive = await downloadPlutoArchive(f.pin, f.directory, { fetch: request })
    const destination = path.join(f.directory, 'extracted')
    await extractPlutoArchive(archive, destination, f.pin)
    for (const [name, bytes] of entries)
      assert.deepEqual(await readFile(path.join(destination, name)), bytes)
    assert.equal(await downloadPlutoArchive(f.pin, f.directory, { fetch: request }), archive)
    assert.equal(calls, 1)
  } finally {
    await rm(f.directory, { recursive: true, force: true })
  }
})

await test('corrupt download cannot enter cache and partial bytes are removed', async () => {
  const f = await fixture()
  try {
    const corrupt = Buffer.from(f.bytes)
    corrupt[corrupt.length - 1] ^= 1
    await assert.rejects(
      downloadPlutoArchive(f.pin, f.directory, {
        fetch: async () => new Response(new Uint8Array(corrupt)),
      }),
      /SHA-256 mismatch/,
    )
    assert.deepEqual(await readdir(f.directory), [])
  } finally {
    await rm(f.directory, { recursive: true, force: true })
  }
})

await test('tampered cached ZIP is rejected before any network retry', async () => {
  const f = await fixture()
  try {
    await writeFile(path.join(f.directory, `${f.pin.sha256}.zip`), Buffer.alloc(f.bytes.length))
    await assert.rejects(
      downloadPlutoArchive(f.pin, f.directory, {
        fetch: async () => {
          throw new Error('Network must not run')
        },
      }),
      /SHA-256 mismatch/,
    )
  } finally {
    await rm(f.directory, { recursive: true, force: true })
  }
})

await test('unexpected ZIP payload is rejected and owned extraction directory removed', async () => {
  const f = await fixture()
  try {
    const bytes = await makeArchive([...entries, ['unexpected.exe', Buffer.from('unexpected')]])
    const archive = path.join(f.directory, 'extra.zip')
    await writeFile(archive, bytes)
    const destination = path.join(f.directory, 'extracted')
    await assert.rejects(
      extractPlutoArchive(archive, destination, {
        ...f.pin,
        sha256: hash(bytes),
        sizeBytes: bytes.length,
      }),
      /Unexpected official ZIP file/,
    )
    assert.deepEqual(await readdir(f.directory), ['extra.zip'])
  } finally {
    await rm(f.directory, { recursive: true, force: true })
  }
})

await test('extracted file hash is checked independently of archive hash', async () => {
  const f = await fixture()
  try {
    const archive = path.join(f.directory, 'valid.zip')
    await writeFile(archive, f.bytes)
    const pin = {
      ...f.pin,
      files: {
        ...f.pin.files,
        'pluto.dll': { ...f.pin.files['pluto.dll'], sha256: '0'.repeat(64) },
      },
    }
    await assert.rejects(
      extractPlutoArchive(archive, path.join(f.directory, 'extracted'), pin),
      /SHA-256 mismatch/,
    )
  } finally {
    await rm(f.directory, { recursive: true, force: true })
  }
})
