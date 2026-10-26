# PurpleWave sb.5 source update candidate

Status: review-only candidate. Source/distribution review and broader game validation remain pending.
The published sb.4 catalog entry remains unchanged.

## Source selection

Candidate source is `08672150433a2c60f5db23e10d2bc15dbc6e302e` (2026-10-15),
the immediate parent of the upstream master revision checked on 2026-10-25.
It is 172 commits ahead of sb.4's `a57d2511cc4f6318c2a2d61a504e8f58a7b090af`.

Upstream head `88c1056aac722fd648ca81e5746da25ed8123c3e` introduces the missing
Aim errors. That commit removes `Techniques.Aim` but leaves its references at
`Decisionmaking/Combat.scala:88,118`, and adds an `Aim(unit)` call in
`Combat/Fight.scala:39` without a corresponding action definition. Those files
match pristine upstream source and are not excluded by the upstream Maven build.
The existing recipe failed with those three Scala compiler errors. The preceding
revision retains the original technique and does not add the unresolved action.
No combat code workaround is included.

The existing SB runtime-isolation patch applies unchanged to the selected parent.
The upstream MIT license, dependency pins, Java 21 requirements and fixed SB
configuration are unchanged. The upstream mjson submodule update is not adopted:
the recipe still builds the separately pinned mjson source in source-lock.json.

## Build and review

Use the committed recipe inputs and a fresh output directory:

```powershell
node tools/build-purplewave.ts purplewave-sb-5-parent-check <jdk-21-home>
node tools/package-purplewave.ts .build/purplewave-sb-5-parent-check purplewave-sb-5 --review
```

The source and patch hashes, recipe revision, toolchain and output hashes belong
to the generated build-info.json. A review archive must retain corresponding
source, notices and build materials. Prior sb.4 gameplay evidence applies to the
older source; this update requires its own validation before catalog admission.

## Packaging fix

A source patch that is also a recipe input is archived by the recipe inventory
instead of being added twice. Source-lock hash checks and committed recipe
verification still run; the archive builder still rejects duplicate entries.
The gameplay sources and runtime dependencies are unchanged by this fix.

## Validation on 2026-10-25

Full source build passed with Temurin JDK 21.0.11 x64 and Scala 2.12.20.
Recipe commit: `0af3074bdbf02df108e7c22d7a9a4ac3094a4f5d`.
Metadata generation check, TypeScript typecheck, lint/format, catalog validation,
and all 66 tooling tests passed. The existing runtime-state codec regression passed.

The local review archive is `dist/purplewave-sb-5-review/purplewave-sb-5.zip`,
20,062,409 bytes, SHA-256
`7bbc47be6b095059e5bbbb60ddbcb7af192a4bca241882e63551a47a51c1e3ce`.
Its 1,183 entries have no duplicate paths; the JBWAPI lifecycle
patch appears exactly once and matches its checked source bytes. All six output
JARs match their recorded sizes and hashes. The bot JAR SHA-256 is
`0b8a7e8be09b91098c9d79b5d24d3836475ce61860770d83f8a20d8c5cef0c57`.

## ShieldBattery gameplay

The extracted review archive ran through existing SB local-build and practice
IPC with Java 21 x64, isolated fresh profiles and Fighting Spirit 1.4.
SB commit: `ff167db43053cead7a991a68c224c8d6bf61066d`; DLL SHA-256:
`026480170ea8a0cc7a5a0ef2ded82ad3ad81148aef8029cfd390ba13874a5a36`. This is a developer-build test, not signed-catalog installation.

| Race and opening | Observations | End scope |
| --- | --- | --- |
| Protoss, default strategy selection | 17 Probes, second Nexus, two Gateways and Cybernetics Core present; 3,910 accepted commands, none rejected at frame 4,320 | Automatically stopped after frame 4,362 |
| Zerg, fixed ZvE4Pool | Spawning Pool, 10 Zerglings and a distant Drone scout; 7,166 accepted commands and 14 rejected at frame 3,600 | Result returned; BWAPI emitted winner=true at frame 3,799, before the stop limit |

Neither bot stdout log contained an exception. Both profiles wrote logs/cache
and history files, and the owned test app/game processes exited. Existing bot
bundles, published catalog and SB DLL were unchanged.

These runs do not establish playing strength, Terran behavior, x86 compatibility,
or saved-state reuse across matches. Source/distribution review and broader game
validation remain pending. The package remains review-only and nothing was pushed
or published.

## Follow-up review on 2026-10-26

The following evidence supersedes the untested scopes above. The reviewed ZIP
and its executable bytes remain unchanged. Machine-readable, sanitized results
are in [purplewave-sb-5-validation.json](purplewave-sb-5-validation.json).

### Rebuild and source review

An independent build used recipe commit
`33fede6ebdb2f647d1f11878377f21c1cebd1f14`, clean pinned source clones,
the same Temurin JDK 21.0.11 x64, and the pinned dependency cache.
Its recipe-input digest matches the original build:
`3b227470968d76b91d3acb60e8bd9e2e07690c5eae62a40f81673c0f768117dd`.
All five prepared source tree IDs and all six output JAR sizes and SHA-256
hashes match the reviewed archive. The executable JAR remains
`0b8a7e8be09b91098c9d79b5d24d3836475ce61860770d83f8a20d8c5cef0c57`.
All 1,129 archived upstream-source files also matched the original prepared
build inputs byte for byte.

The automated review examined the upstream delta from sb.4, the rebased
isolation patch, compiled source I/O paths, configuration, dependencies,
recipe, package contents and notices. The delta spans 329 files, including
tests and development scripts. The new external-data input is the fixed
`bwapi-data/AI/fixedbuilds-by-opponent.json` file: opponent names select a JSON
entry, rather than constructing a path. The fallback is the existing fixed
`fixedbuilds.txt` path. Upstream's six exact-match/case-insensitive/fallback
checks passed against the packaged runtime. Additional malformed-JSON and
typed-default checks are recorded with the validation results. The existing
filename/history codec regression passed.

The history loader, serializer, filename codec and persistence destinations
are unchanged by the upstream update. Placement-cache format changes from
3 to 5; incompatible caches are rejected and regenerated. This is separate
from opponent-history format v5, which remains compatible. Cache size and
structure bounds, temp-file replacement and profile-relative paths remain
in place. The visualizer auto-launch path remains removed; the fixed package
configuration disables visualizer, debug, human and livestream modes.
The recipe compiles Java/Scala directly and does not execute upstream Maven
plugins, Python helpers or launch scripts. Dependency source pins and runtime
JARs are unchanged from the prior review. The optional mjson URL/schema APIs
are not called by the reviewed configuration path.

PurpleWave's MIT license and all dependency license texts remain unchanged.
All 16 declared package notice paths exist. Fifteen notice files match sb.4
byte for byte; the libffi notice differs only by CRLF-to-LF conversion, with
identical notice text. Its sb.5 SHA-256 is
`2c9c2acb9743e6b007b91350475308aee44691d96aa20eacef8e199988c8c388`.
The manifest discloses the source update and rebased patch; source, patch,
recipe and dependency materials are included. Java is supplied separately.

### Gameplay and persistence

Tests used the existing SB local-build/practice flow, isolated profiles,
the extracted review ZIP and Fighting Spirit 1.4. SB revision and x64 DLL
match the earlier tests. The x86 DLL SHA-256 is
`2593991d264c9f4ab0aea48305109457d9d23ad0ad3e13e1a944e5ec387027bc`.
These are developer-build tests, not signed-catalog installation.

| Test | Observed result | Scope |
| --- | --- | --- |
| Protoss versus Zerg, x64, separate work profiles | Protoss won at frame 15,228; Zerg lost at 15,224. Both callbacks preceded observer cleanup. Both JVMs closed without stdout exceptions. | Naturally completed 1v1; Zerg used `ZvE4Pool`, Protoss selected its normal strategy. |
| Terran, x64 | `TvE3Fac` produced SCVs, Barracks, Supply Depot, Marines and a Bunker; 1,194 accepted commands, zero rejected. History saved. | Opening and quit lifecycle only; player menu exit ended the run at frame 2,784. |
| Protoss, x86 | Commands and economy advanced; 931 commands accepted, zero rejected; callback at frame 1,496. | Deliberate bounded smoke test, followed by history reload. |
| sb.4 history upgrade and next-match reuse, x86 | A copy of an authentic sb.4 history row survived the first sb.5 match and the next sb.5 match. Each match added one actual record. | Original sb.4 profile preserved; identical read/write input rows were saved once. |
| Path-like opponent name and corrupted history copy | `../CON,escape` became `%2E%2E%2FCON%2Cescape` in profile filenames. A copied authentic history row truncated to five bytes was logged and skipped; gameplay continued past frame 720. | Deliberate negative test; expected deserialize error, not an unexplained bot exception. |
| Abrupt session termination and recovery | Forced termination of the owned player client produced `Local game client 0 disconnected`; session cleaned up. The same bot profile restarted and played past frame 720. | Process-loss recovery; not a native access-violation crash test. |

The full-match bridge totals were 68,411 accepted / 125 rejected commands
for Protoss and 72,560 accepted / 50 rejected for Zerg. Neither log contained
an unsupported/malformed-command warning. The earlier Zerg run's final total
was 8,082 accepted / 16 rejected; the 14 figure above was a frame-3,600 sample.
Current bridge diagnostics do not distinguish every stale actor/target or
other rejection path. The causes remain unclassified; no claim of zero
command loss or complete BWAPI compatibility is made.

The authentic sb.4 history input SHA-256 was
`05a24b09da3ff4a3a9a012189d1f3047658a1a0cb47575aeb57686eba14b7605`.
Its timestamp `1792953750558` appears once after each test, alongside the
actual sb.5 records `1793016328198` and `1793016713062`. The read input
remained unchanged. No history rows or results were fabricated. Earlier
state and reset snapshots remain private and are not part of recipe inputs.

A subsequent manual reset moved the disposable BYO profile's read/write
directories into a retained snapshot and restored the package's empty
baseline. The same x86 profile restarted past frame 720 and saved only its
new actual record (`1793017779968`). The prior three history rows remained
in the snapshot. This verifies baseline restoration and bot startup, not
the installed-package `botLibraryResetLearning` action or its UI.

Two preliminary player runs ended through `GameMenu`, `AbrtMenu`, then
`Quit2Mnu`, without an earlier SB leave request in the game log. The origin
of that input is unresolved. Another observer run was explicitly quit by
the user at frame 9,347. None counts as a naturally completed match.
The successful full match used a silent observer session and no frame stop.

### Runtime tracing and limits

A running Terran JVM sample had no TCP/UDP endpoints. Its observed child
was Windows `conhost.exe`. JFR recorded 3,676 file-read and 910 file-write
events, with no socket or process-start events during that recording.
The negative test recorded from JVM startup through shutdown: 7,415 file-read
and 1,108 file-write events, with no socket or process-start events. Observed
paths covered runtime/package JARs, Java logging configuration, assigned BWAPI
pipe, fixed bot configuration, copied history, profile outputs and JNA native
extraction under the isolated test TEMP directory. Encoded outputs stayed
inside the profile. JFR instrumentation was added only to that test's JVM
arguments; the review archive was not modified.

JFR is not a complete Windows syscall or registry trace, and source review
does not provide an OS sandbox. Native dependencies retain the prior reviewed
pins. This review does not establish arbitrary configuration safety, playing
strength, a Terran full match, Terran/Zerg x86 coverage, Random, teams or FFA.
The background-client crash IPC did not route to its local manager, so the
recovery test used explicit termination of the owned player process instead.
Installer/UI reset and signed-catalog installation remain separate checks.

Metadata generation, TypeScript typecheck, lint/format, catalog validation
and all 66 tooling tests passed on the PR branch. No pinned working bot,
licensed game asset, published archive or catalog was replaced. Testing still
depends on the gathered-resource fix in SB PR #1831.

### Admission status

The automated source/license audit found no newly introduced reachable
networking, child-process launch or out-of-profile data path in the reviewed
fixed configuration. The candidate now records prototype-tested compatibility
and the exact scopes above. Source review and local-distribution admission
remain pending: the evidence does not include full native syscall/registry
tracing, and command rejections still lack a complete explanation. These
results do not substitute for admission approval or public-competition review.
The published sb.4 catalog and the sb.5 review archive remain unchanged.
