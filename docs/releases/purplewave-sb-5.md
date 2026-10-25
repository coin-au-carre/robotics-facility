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
