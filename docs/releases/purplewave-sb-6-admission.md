# PurpleWave sb.6 admission follow-up

**Production approval is withheld.** Remaining installer/UI checks passed, but
Terran factory games exposed unsupported Lift and Spider Mines commands in the
ShieldBattery bridge on both architectures. ShieldBattery also retains an explicit
development-only runtime gate. Source review and local distribution are approved
for scoped experimental use of the pinned inputs below; that is not production
approval. No publication was performed.

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

A separate local installer-test package was assembled without overwriting the
review ZIP: 20,065,845 bytes, 1,186 entries, SHA-256
`60f80501b9d898b0b392df624e35d15df4bcb3749338e77b8e78820405e980b4`.
Every entry except `package.json` is byte-identical to the review archive. Its
descriptor records the source/distribution admission and the then-available
natural-match evidence. It was not added to the published catalog. The subsequent
factory results below supersede its broad format claim; do not publish this
artifact as production-approved on the tested bridge.

Two fresh real Electron sessions installed those exact bytes from local HTTPS
catalogs signed with independent ephemeral Ed25519 keys through the repository
publisher. Test bootstraps trusted only the exact loopback TLS leaf fingerprint;
normal verification remained in force for other hosts. The app rejected a changed
catalog signature without losing the valid cache, and rejected changed archive
bytes before installing. It then installed the intact archive, opened a declared
notice and launched through `practiceGameStart` using catalog keys.

On each architecture, two installed instances played a bounded first match, which
created separate `work` and `work-2` histories. The real bot-details dialog was
opened through the app's existing development dispatch surface, without faking
account authentication. Its actual **Reset learning** button was disabled while
playing; direct reset IPC also refused. After normal stop, clicking the actual
button and confirmation restored both profiles' empty read/write baselines.
The same UI checks passed after the factory match. All installed files
remained unchanged; the library retained the installation with no active leases
or install failures. These establish local signed installer/UI behavior, not
public staging/CDN delivery or production-key configuration.

## Factory-strategy compatibility blocker

After the first reset, only the private profiles received `TvE3Fac` as a strategy
selection file; the installed package was unchanged. Both Terran instances on each
architecture played to approximately frame 10,000 and were then stopped gracefully.
These are bounded progression tests, not additional natural-match results.
All 42 common sync probes through frame 9,840 agreed in each session.

Read-only stable BWAPI snapshots from one bot per architecture established actual
gas collection, up to three Factories and two Machine Shops, Marines, Vultures,
and tanks reaching Siege Mode. Unit IDs were checked against the packaged JBWAPI
enum. This is stronger progression evidence than selecting the strategy alone.

However, all four bots repeatedly attempted unsupported commands:

| Game architecture / instance | Lift (kind 24) | Spider Mines (kind 41, tech 3) |
| --- | ---: | ---: |
| x64 first | 2,004 | 89 |
| x64 second | 2,168 | 39 |
| x86 first | 2,099 | 52 |
| x86 second | 2,168 | 171 |

Lift came from owned Barracks (unit type 111). JBWAPI's `UnitCommand.lift` leaves
unused coordinates at `Position.None` (`32000`, `32032`); the command kind is
absent from the bridge encoder. Pinned BWAPI's `GameCommands.cpp` routes Lift to
`BW::Orders::Lift` (`0x2f` plus its four zero payload bytes). Spider Mines attempts
came from owned Vultures (type 2), with in-map target pixels and tech ID 3. The
bridge's positional-tech encoder currently admits only Scanner Sweep and EMP.
These are genuine missing bridge behaviors, not merely stale-target guards.
Fresh upstream `feature/bwapi-compat` at
`a096b05e199ac0aaa6d46d7da21b8833acfd3218` also lacks these encodings.

Source/distribution approval covers the exact bot and its experimental local
profile. Candidate 1v1 support is set back to `experimental`, and no production
approval status is set. Fix and verify the separate ShieldBattery bridge commands
on both architectures before reconsidering approval; test lift/landing behavior
and actual Spider Mine placement, not command acceptance alone.

## Deployment scope

The offered scope is ordinary 1v1 with assigned Protoss, Terran or Zerg on the
tested bridge. Random selection, teams, FFA, unusual maps and complete late-game
BWAPI conformance remain unverified. Human difficulty remains uncalibrated.
Deployment needs gathered-resource and train-cancellation fixes from ShieldBattery
PRs #1831 and #1837 plus the newly identified missing command support. Current
upstream `app/app.ts` rejects non-development local/practice starts, and
`game/src/lib.rs` declares BWAPI under `#[cfg(debug_assertions)]`. Enabling a
supported production runtime is a separate application decision; changing a bot
catalog flag does not remove those gates. No existing published catalog or package
changed as a result of this local review.

All test-owned Electron, SC:R and Java processes were confirmed absent after cleanup.
Metadata/catalog validation, typecheck, lint/format and all 66 tooling tests passed.
