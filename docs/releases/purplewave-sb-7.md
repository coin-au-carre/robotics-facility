# PurpleWave sb.7 metadata-only successor

Approve exact `purplewave-sb-7` for assigned Protoss/Terran/Zerg local 1v1 under the
same reviewed runtime/client conditions as sb.6. This successor reconciles project
attribution with the archive descriptor while preserving every runtime byte.
Original sb.6 and all historical packages remain unchanged.

## Why a successor

The production fragment at [PR #2 head
fd0985f](https://github.com/coin-au-carre/robotics-facility/commit/fd0985fabb125fd5179633d8bf089605a6c293be)
corrected downstream project attribution but retained sb.6's original archive and
manifest digests. The fragment's package object therefore differed from the
reviewed ZIP's embedded descriptor. The maintained archive verifier requires both
the original digest and exact descriptor equality; that fragment cannot publish
as written. The original sb.6 archive was not overwritten or silently relabelled.

## Exact new artifact and delta

- New ZIP SHA-256: `ec3089ee4e93632e1940ce0fdb85d01e547fd593c62725f7e6a7e0aedb7f6b34`.
- New ZIP size: 20,068,672 bytes.
- Parent sb.6 ZIP: `60f80501b9d898b0b392df624e35d15df4bcb3749338e77b8e78820405e980b4`.
- Committed metadata recipe: `49800da` and
  [tools/repackage-purplewave-metadata.ts](../../tools/repackage-purplewave-metadata.ts).
- [Member and JAR hashes](purplewave-sb-7-validation.json) record the independent
  archive comparison. All 1,185 parent members other than `package.json` are
  byte-identical; no parent member is removed.
- Only descriptor fields `releaseId`, `version`, and the downstream update's
  `modifier` change. The release is `purplewave-sb-7`, version `2026.09.22-sb.7`,
  with project attribution `ShieldBattery`.
- Two files add metadata-repack provenance and the committed recipe source.
  Original source, corresponding-source rebuild instructions, build record,
  runtime revision marker, dependencies, configuration, licenses, and notices
  stay byte-identical. This is not a compiler rebuild or a new bot implementation.

The guarded recipe verifies the exact parent size/digest, embedded descriptor,
archive structure, CRCs, and required files, then uses maintained release assembly.
It verifies committed recipe bytes before assembly and refuses existing output.
Reproduction requires the exact parent ZIP and the recorded recipe commit:

```powershell
node tools/repackage-purplewave-metadata.ts C:\review\purplewave-sb-6.zip
```

The resulting `dist/purplewave-sb-7/` contains the immutable ZIP and catalog fragment.
This command does not sign, upload, or publish. A different parent fails integrity
checks; changing bot/source/configuration inputs requires a new review/build.

## Evidence carried forward without enlargement

Exact sb.6 source/artifact admission and release evidence remain at
[the reviewed PR #2 revision](https://github.com/coin-au-carre/robotics-facility/blob/f31368c/docs/releases/purplewave-sb-6-production.md)
and its [machine-readable runtime evidence](https://github.com/coin-au-carre/robotics-facility/blob/f31368c/docs/releases/purplewave-sb-6-production-validation.json).
All six runtime JARs, including PurpleWave JAR
`e2bfd2519e07e6597bd2159b6044df6c2fca11ebc2b86a532f28e182bd452013`,
are unchanged. That supports retaining the exact runtime's source review,
corresponding-source rebuild, state reload/reset, and gameplay evidence.

Those records cover natural assigned-race 1v1 outcomes and saved-row/callback
agreement on release x86/x64, bounded Terran production through frame 15,000,
and genuine signed isolated installation/tamper/reset checks with development
trust configuration. They do not establish public production-key/CDN delivery.
No new gameplay, natural result, state file, or installed-package test is claimed
for sb.7; the new evidence is exact-member and metadata integrity verification.

## Runtime and rollout conditions

Requires Java 21 x64 and a client integrating SB
[#1831](https://github.com/ShieldBattery/ShieldBattery/pull/1831) for resource totals,
[#1838](https://github.com/ShieldBattery/ShieldBattery/pull/1838) for supervised
release practice and fixed production catalog trust, and
[#1841](https://github.com/ShieldBattery/ShieldBattery/pull/1841) for flight/mines.
Training cancellation #1837 is already merged into the feature target. Release
approval does not implement missing client commands.

Assigned Protoss/Terran/Zerg local 1v1 is the reviewed scope. Random, teams, FFA,
unusual maps, exhaustive late-game command behavior, human difficulty calibration,
and public competition remain unverified. Existing native guard rejections and
historical logging limits remain in the original report. No OS sandbox is claimed.

PR #4 now includes this new exact catalog entry alongside six other approved
releases; PR #2 retains source/build history and the original runtime review.
Before live availability, maintainers must upload this exact sb.7 ZIP as a versioned
prerelease, publish a new signed staging revision, verify CDN bytes and receipt,
then promote that revision through the protected workflow. No release upload,
signing, workflow dispatch, or publication occurred here. BananaBrain and Pluto
retain their previous pending/development-only decisions.
