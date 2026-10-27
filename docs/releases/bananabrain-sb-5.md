# BananaBrain sb.5 local-practice admission

Review date: 2026-10-27. Source and local-distribution review approved for the
inputs and scope below. Final admitted archive/installer checks are recorded separately.
Scope: assigned Protoss, experimental local 1v1, Win32 native bot with x64
StarCraft on Fighting Spirit 1.4. Human strength remains uncalibrated. Teams, FFA,
random/Terran/Zerg assignments and x86 StarCraft are unverified. Public tournament
permission is not granted by this review.

## Exact inputs and artifact chain

Fresh guarded review package `bananabrain-sb-4` was built from committed recipe
`09a3008da5e07e3f1b2e8a0e879f7a1387828615`, recipe digest
`9f47fc7546fdb1b78397b2c7f983d6ca02409d008f1a53f89d83be05624dab8e`.
The source import stays `f66d7336ca61a0a0a1a984fad55c162d942694e4`;
storage patch SHA-256 is
`62917ef24f897ae585f531ce96f664faa47023228f238aa0598e6eb7f61e12e6`.
BWAPI stays at `7687da8abc4726f8366401f11ab648d421385793` with the locked instance
discovery patch. Prepared tree IDs and toolchain are in the validation record.

Review ZIP: 3,292,765 bytes, 296 entries, SHA-256
`845af610e6fef89c7bcb19499edecd58359cd702106018b30de5a1809556b268`.
Executable SHA-256:
`f4e88bba24eca90aa4251c94bc952ee9dc7a754ff7f040633f7aacbc7a2f6098`.
All 12 original AI configuration/training files match the pinned author submission.
The source-only rebuild used extracted source, installed VS 2022/MSVC 19.35,
Windows SDK 10.0.22000.0 and CMake 3.25.1. No existing binary/library, source fetch
or package-manager step was used. Network was not blocked at OS level. Rebuilt
executable SHA-256:
`37a6eb939180daac8c063ce8935de1dbc6d06aa4675ae98073e883d98a3b69d8`.
Byte-identical builds across directories are not claimed.

The admitted sb.5 package carries the same tested executable, patched source,
original AI bytes, build record and existing license notices. Its new identity,
review metadata, project modifier attribution, current BUILD instructions and
additional admission notice are packaging changes. Earlier review archives stay
immutable. Archive delta and final hashes are recorded below after assembly.

## Source and dependency review

Review covers the imported BananaBrain/BWEM/JPS sources, locked storage and BWAPI
patches, native host, source-built BWAPI client, fixed configuration, original AI
assets, native CMake inventory, package assembly and resulting archive. The current
storage-patch delta from the earlier reviewed recipe changes three comments;
current prepared source and new binary were nevertheless rebuilt and tested.
Historical findings and their fixes remain in
[the source review](../bananabrain-source-review.md).

Active mutable paths are bounded opponent-result and map-mining files under
`bwapi-data/read` and `write`; original assets stay in `AI`. Unsafe opponent bytes
are encoded; device names, traversal, reparse paths and hardlinked inputs are
rejected. The fixed shipped configuration cannot be overridden through mutable
state or `schnail.env`. Strategy code and author training data are retained.
The helper caps files at 8 MiB, records at 100,000 and lines at 4,096 bytes.
Rejected output does not fall back to older read input. Oversized saves retain
previous history instead of silently pruning it.

No active bot-source network, updater, registry, dynamic-loader, shell or helper
process call was found. Optional debug-file writes and BWEM map printing remain
outside the compiled configuration. BWAPI uses assigned local shared memory and
named-pipe IPC; its RemoteProcess utility has no selected host/client call site.
A live endpoint snapshot found zero bot-owned TCP/UDP endpoints and only the
Windows console child. Source review, file inventories and endpoint snapshots are
scoped evidence, not exhaustive system-call tracing or an OS sandbox guarantee.

## Storage failure and isolation checks

The exact packaged methods passed the maintained MSVC harness: malformed/oversized
records, integer overflow, unsafe names, stale inputs, CSV delimiters, failed
replacement, input/output bounds, junctions, hardlinks and all 11 original
training/mining files. The helper used by that harness matches packaged bytes.

The new [fault harness](../../bots/bananabrain/runtime-tests/faults.cjs) compiles the
actual packaged helper. It passed injected `ERROR_DISK_FULL` write/flush failures,
short writes and denied replacement. Each failed save retained the complete prior
file; subsequent reload/save succeeded and handled-failure temporary files were
removed. This injects I/O failures; it does not exhaust a physical disk.

Parent terminated only its spawned storage child immediately before and after
atomic replacement. Reload observed respectively the complete old/new history,
then saved successfully. A pre-replacement crash leaves an ignored temporary file,
not partial history. Reset removes mutable leftovers while idle. Two simultaneous
processes also completed 100 writes/reloads each in separate profiles without
contamination. Shared-profile writers are unsupported; no cross-process storage
lock is claimed. Interrupted writes do not establish crash recovery for every
StarCraft/BWAPI subsystem.

## Exact-runtime gameplay

The extracted review package ran through existing SB local-build APIs with fresh
isolated profiles, original AI assets and a hash-checked cached map. App source:
`04dc6ee699b4cfbf91dc66a1d0b61d84ccfc2c89`. Release DLL integration source:
`07595bd56318a5ad4272004c4e3cffac2bd13722`; loaded DLL hash is in the validation
record. That integration contains required gathered-resource translations.

| Run | Observed result |
| --- | --- |
| Natural match against passive Terran control | Protoss win at frame 8,683; 3,925 accepted commands, two rejected |
| Two independent Protoss instances | Both reached frame 1,200; 57/59 accepted commands, zero rejected |

The natural winner callback occurred before cleanup and agrees with the saved
result row's win flag and frame. No frame limit or scripted resignation ended that
match. No renderer result event was captured. Rejection reasons were not captured;
this is completion evidence against a passive control, not active-human strength.
Paired tests were deliberately stopped gameplay checks, not completed 1v1 results.

The forced bot-exit test reached frame 1,200 in two independent instances, then
terminated one owned bot process. Normal supervisor stop removed the remaining
owned test processes; cleanup census found zero owned app/game/bot processes.
This is forced-process-exit evidence, not an unhandled game-engine crash.

## Distribution and notices

The BananaBrain License permits copying, modification and redistribution with its
notice retained. Public StarCraft tournament submission separately requires written
author permission; none is recorded. Local supervised practice approval does not
satisfy that condition. Original Johan de Jong attribution stays intact.

| Component | Packaged notice/source |
| --- | --- |
| BananaBrain and original AI assets | `notices/BananaBrain-License.txt`; complete `source/bananabrain` |
| Modified BWEM-community | `notices/BWEM-License.txt`; original MIT notice and source retained |
| JPS | Original public-domain dedication in `source/bananabrain/src/Source/JPS.h` |
| Source-built BWAPI 4.4 | `notices/BWAPI-LGPL-3.0.txt`, `notices/GPL-3.0.txt`; patched source and patch |
| Native host/helper | `notices/ShieldBattery-MIT.txt`; source, license and CMake recipe |
| Downstream changes/review | Original `notices/RELEASE.txt` plus new `notices/ADMISSION.txt` |

The original RELEASE review notice is preserved verbatim, including its historical
pending-review sentence. ADMISSION and the immutable descriptor state the current
scoped decision. The app must retain and display all notices offline, upstream
authorship and project modification disclosures.

Corresponding source is inside the same versioned ZIP as the executable, including
patched bot/BWAPI sources, dependency headers, source-lock/patches and native build
materials. Offline source rebuilding verifies a user can modify/relink the static
BWAPI combined executable. Preserve modification/debugging rights and equivalent
source access when publishing; a mutable upstream URL alone is insufficient.
Review decision covers these exact inputs and delivery arrangement; it grants no
new rights in upstream work.

## Client and delivery limits

Use the reviewed SB native bridge and per-instance app-owned writable profiles.
[SB #1831](https://github.com/ShieldBattery/ShieldBattery/pull/1831) supplies accurate
gathered resources; [SB #1838](https://github.com/ShieldBattery/ShieldBattery/pull/1838)
supplies supervised release practice and fixed production catalog trust. The local
integration used for gameplay is recorded above; it is not presented as a fresh
build of every latest client PR. Do not ship an older integration merely because
this bot ran on it.

Admission is separate from publication: maintainers must upload this exact new
archive, publish a new signed staging revision, verify delivery and activation,
then promote through protected production workflow. No public release upload,
catalog signing, workflow dispatch or production promotion is part of this review.
Pluto remains development-only and is absent from this package/catalog entry.
