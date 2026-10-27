# Pluto v2 shared-catalog integration review

Status: review artifact, not a published catalog release. Source admission remains
pending maintainer review; no maintainer approval or production activation is claimed.

## Package and source

Release `pluto-sb-1` uses the adapter-only `pluto-v2` runtime. Facility ZIP is
626,958 bytes, SHA-256
`d96b2b12744892e9e7cbf185f8d41457d225f15d3c3ce7c99e3e4b41bf6cf646`.
Embedded package manifest SHA-256:
`3136b0daf0495f362f88fcdbd23c5aa4f946ae691f1c7ba8f1e62788d89388c5`.

The ZIP contains 205 matching compiled source/dependency files, build instructions,
MIT/LGPL notices and modification disclosures. Source snapshot:
`8cd1f60680efcb81ac92cd7221d4ad45b4ed5606`; packaging recipe:
`77fcc198cdb53412a780a2bc7f9757d6beff0733`. Source-built companion SHA-256:
`854492a20a8e7f38640e8dc097970fda6741393ff076b5e167d6cf6b2fbce440`.
A subsequent recipe guard also rejects incomplete source inventories.

Original Pluto DLL, inference executable and model weights are absent from this
archive. Installer fetches the 309,686,477-byte official v2.0 ZIP directly from
`https://github.com/tscmoo/pluto/releases/download/v2.0/pluto-v2.0.zip`.
ZIP SHA-256:
`17544fc3ab32e3e2823646887e021cf998362348d73740817e94d1a78aa6209d`.
The signed adapter archive's upstream pin document must exactly match SB's supported
pin. Installation verifies ZIP and individual original files before promotion:

| File | SHA-256 |
| --- | --- |
| `pluto.dll` | `603fea2f77a4b652f29b832c2977cf5bc9d83bbcb927febd04499220020ce3bc` |
| `pluto/pluto_infer.exe` | `3f2814a51034b9817b396e52b38d30341ede506a2baedc7cbc11cb6ebcece2cc` |
| `pluto/pluto_weights.bin` | `c53a559b4910777dc0e1c561c0d89d243091f465d8e9fc0f5f72e56597067d86` |

## Catalog installation evidence

A fresh Windows Electron profile used an Ed25519-signed loopback development
catalog and the normal **Download** button. No local-build entry or model folder
was imported. Installation fetched original models from the official release,
verified all four runtime asset hashes and displayed **Installed**. Total download
size was 310,313,435 bytes. A second test canceled during model download, verified
empty staging/download directories and no installed record, then successfully
retried through the same Download button.

Development signing used a fresh in-memory key. Production trust keys, shared
catalog and signing workflows were unchanged. Candidate source-review status stayed
pending throughout these tests. This verifies signed installer behavior, not
production admission or publication.

Initial attempt ran out of disk space and stalled during extraction. Its result is
excluded. A later harness attempt installed successfully but failed on an ambiguous
Installed selector; corrected test passed. Evidence retains those failures.

App typecheck/lint and 252 focused tests passed on current upstream bridge target.
Facility typecheck/lint/metadata validation and 73 tests passed. A deliberately
incomplete source inventory was rejected without producing an archive.

## Admission and rollout

[Review catalog](../../bots/pluto/catalog-review.json) retains exact reviewed ZIP
and descriptor; it is separate from active `catalog/catalog.json` because source
admission is still pending. It does not grant original-model redistribution.

Prior finite fog, scalar, neutral-cache, startup and binary reviews establish bounded
compatibility evidence. They do not prove full closed-binary noninterference or OS
containment. Runtime uses native code in its own local game client and owns its
helper job; working directories and job ownership are not a security sandbox.
Human difficulty remains uncalibrated, public competition unreviewed, and teams/FFA
unsupported. Experimental scope is 1v1 Melee, at most 128 by 128 tiles, Windows x64
with AVX2; Pluto game client uses x86.

Rollout order: review/merge SB runtime and installer; ship client update; approve
facility admission; publish immutable adapter/source artifact; add approved release
to active catalog; use protected main staging workflow and verify readback; then
promote exact staging bytes to production. Older clients cannot parse `pluto-v2`
runtime, so catalog activation must follow client rollout. Contribution access does
not include upstream signing secrets or permission to merge protected branches.

Local evidence: FBH
`StarCraftLab/research/compatibility/pluto-v2-catalog-integration/`, including
`signed-install-3.json`, `signed-cancel-1.json`, `app-checks-final.json`,
`facility-checks-final.json` and `inventory-rejection.json`. Private profiles and
raw logs are excluded from repository.
