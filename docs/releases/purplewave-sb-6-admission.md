# PurpleWave sb.6 admission follow-up

Source review and local distribution are approved for the pinned source, patches,
dependencies and fixed package configuration described below. Production readiness
still awaits the installed-package and UI checks recorded in the final section.
No publication is authorized by this report.

## Exact reviewed inputs

The original review archive remains unchanged: SHA-256
`c19bd7d3761621d221e652126208c7a1de52e8b5f15fd79e673cb8e5525875a1`.
It contains 1,186 unique entries and all 16 declared notices. All extracted files
were compared with the archive before testing. The bot JAR is
`e2bfd2519e07e6597bd2159b6044df6c2fca11ebc2b86a532f28e182bd452013`.
Sources, ordered patch digests, recipe and runtime JAR hashes remain those in
[sb.6 validation](purplewave-sb-6-validation.json).

An independent offline build compiled the archive's 118 Java, 2 macro and 961
Scala source files with JDK 21.0.11 and hash-verified cached compiler/dependency
JARs. It used the packaged build helper, no source clones, Maven plugins or network
fetches. All six output JAR sizes and SHA-256 hashes matched the review archive.
The compiler emitted 29 Java and 6 Scala warnings; these are existing upstream
Unsafe/deprecation/type warnings, not failed compilation.

## Source, effects and distribution decision

The sb.5 upstream update review, runtime-isolation review, dependency/license
inventory and scoped native/JFR evidence remain applicable to their unchanged
inputs. This review independently compared complete sb.5/sb.6 source and JAR
inventories: only `src/Micro/Actions/Basic/Gather.scala` changes among upstream
sources; only `Micro/Actions/Basic/Gather$.class` changes among compiled classes.
All five runtime dependency JARs are unchanged. The added nonempty-threat guard
has no filesystem, registry, process or network operation. Its compiled boundary
regressions and source rationale remain in the sb.6 review.

The fixed configuration disables human, ladder, livestream, tournament,
pretraining, debug and visualization modes. Visualizer process launch remains
removed. Reviewed active effects are package/configuration reads, assigned BWAPI
shared-memory/named-pipe IPC, encoded opponent histories, versioned placement
caches and logs below app-owned working profiles. Optional mjson URL/schema APIs
and standalone visualizer/research entrypoints are not invoked by the packaged
main path. Configuration and opponent-name handling retain the reviewed fixed
paths and encoding. No new reachable networking, child-process launch or
out-of-profile write path was introduced by sb.6.

PurpleWave's MIT notice, component notices, corresponding patched source,
source/patch/dependency locks and rebuild recipe are included. All original
license/NOTICE materials from sb.5 remain present; the modification disclosure
adds the Gather guard. Java is not bundled. This supports scoped local
redistribution of this exact build; public competition remains unreviewed.

Native WPR evidence is scoped to the byte-identical sb.5 dependency/runtime
paths and documented capture limits. It is not represented as a new sb.6 native
capture or a complete Windows syscall trace. Previous undecoded/registry-attribution
limits remain explicit. Neither source review nor signatures provide an OS sandbox.

## Remaining Terran gameplay

The exact sb.6 review archive ran two new natural Terran-versus-Zerg 1v1 matches
on Fighting Spirit 1.4. Both used fresh separate profiles and the previously
recorded diagnostic SB build `fc315a61f9bd175eaf34a9284bfd96e4224a5c98`.
DLL hashes matched the prior sb.6 x86/x64 tests. The opponent used `ZvE4Pool`;
Terran's default selector chose `EvEWorkerRush` on both runs.

| Game architecture | Terran | Zerg |
| --- | --- | --- |
| x64 | Natural loss at 4,994; 6,384 accepted / 14 rejected commands | Win at 4,999; 11,413 / 3 |
| x86 | Natural loss at 5,490; 6,485 accepted / 5 rejected commands | Win at 5,495; 17,533 / 16 |

Each saved-history winner agrees with its native callback. No caught bot
exception occurred. Every rejection was classified by diagnostic logs as native
actor/target availability, ownership or visibility; none was an unsupported or
malformed command. Natural results preceded observer cleanup. These establish
Terran worker-rush compatibility and natural lifecycle, not every Terran tech
path or playing strength. Earlier sb.6 evidence supplies natural Protoss/Zerg
matches on both architectures and later real history-file reload/deduplication.

Machine-readable results, exact hashes and offline rebuild inventory are in
[admission validation](purplewave-sb-6-admission-validation.json).

## Installed package and UI

Pending final verification. The final admitted package will retain the reviewed
binaries, configuration, source, patches and notices; only its descriptor will
record admission and verified 1v1 evidence. The review archive is preserved.
Signed local-catalog installer tests are separate from a public staging/CDN
publication and will be identified as such.

## Deployment scope

The offered scope is ordinary 1v1 with assigned Protoss, Terran or Zerg on the
tested bridge. Random selection, teams, FFA, unusual maps and complete late-game
BWAPI conformance remain unverified. Human difficulty remains uncalibrated.
Deployment needs gathered-resource and train-cancellation bridge fixes from
ShieldBattery PRs #1831 and #1837. No existing published catalog or package changes
as a result of this local review.
