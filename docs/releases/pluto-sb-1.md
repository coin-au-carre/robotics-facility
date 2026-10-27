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

## Release-host gameplay evidence

SB revision `6d9cae445` fixes the production-client practice gate: resolved Pluto
launches are supported, while external BWAPI bots still require development DLLs.
254 app tests, TypeScript and targeted lint passed. Both game architectures built
through `build.bat release` with dependency notices generated. Rust tests passed on
both architectures (633 each), both-target Clippy passed with warnings denied, and
formatting passed. Initial Clippy failure was corrected by moving Pluto's `impl`
before the test module. Removing `cfg(test)` code leaves the release token sequence
unchanged; no gameplay behavior was changed after the recorded live run.

A fresh Electron process reopened the catalog-installed profile with
`ELECTRON_IS_DEV=0`. Main/renderer bundles were development bundles; this explicitly
exercised Electron's actual production-mode launch gate and catalog trust, not a
signed packaged production distribution. Installed release remained usable despite
the different catalog trust. **Start game** launched human Protoss on x64 and Pluto
Terran on x86, Fighting Spirit 1.4. Companion completed startup and submitted 54
translated command packets. Graceful manual stop reached finished state, released
library lease and left zero owned inference helpers. Immutable installed model and
adapter hashes were unchanged. No natural match completion, learning reload or
strength result is claimed by this run.

| Loaded release DLL | SHA-256 |
| --- | --- |
| x86 Pluto | `505d582bd2e71b0d6a7acf198fc1d3b87fa137ab970fe786bb485eec020037b9` |
| x64 human | `a0065229172a01b3a1a90401aead76fe07aa6da96691f549bbf1143308d89829` |

Evidence: `signed-production-2.json`. First production-mode harness attempt stopped
before launch because it omitted bot-library navigation; corrected run passed.
Runtime command evidence and game DLL hashes belong to this exact run, independently
of earlier historical .10 matches.

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
`facility-checks-final.json`, `inventory-rejection.json`, `signed-production-2.json`,
`app-checks-production-gate2.json` and `fresh-release-build-notices.json`. Private profiles and
raw logs are excluded from repository.

Integration PRs: [ShieldBattery #1843](https://github.com/ShieldBattery/ShieldBattery/pull/1843)
and [robotics-facility #5](https://github.com/ShieldBattery/robotics-facility/pull/5).
Public review-archive readback matched the tested SHA-256 exactly. Consolidated local
record: `pluto-v2-catalog-integration/validation.json`, with source-layout correction
tracked separately in `lint-move.json` and `lint-fix-checks.json`.
