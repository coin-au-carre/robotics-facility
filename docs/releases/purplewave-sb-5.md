# PurpleWave sb.5 source update candidate

Status: review-only candidate. Source/distribution review and game validation are pending.
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
