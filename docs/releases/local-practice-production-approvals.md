# Scoped local-practice production approvals

This proposal reviews the seven existing catalog bots and BananaBrain against
release-specific source, artifact, gameplay, installer, and state evidence. It adds
production availability for six existing releases, the metadata-only PurpleWave
sb.7 successor, and newly admitted BananaBrain sb.5 from PR #3. This approval PR
preserves existing immutable package descriptors. Fresh BananaBrain build, gameplay,
installer and state evidence comes from its linked admission PR.
The [machine-readable decisions](local-practice-production-approvals.json) pin the
archive, manifest, and canonical package digests and record per-release limits.

Production approval here permits distribution for supervised local practice within
the reviewed profile. It does not turn an experimental format into a verified one,
calibrate human difficulty, approve public competition, or certify every map/command.
Known experimental capability labels remain in the package and app. The scope in
this report is an evidence boundary, not a newly implemented runtime format lock.

## Decisions

| Bot | Exact release | Decision | Reviewed scope and material limits |
| --- | --- | --- | --- |
| ZZZKBot | `zzzkbot-sb-1` | Approve | Assigned Zerg, ordinary local 1v1. x64 natural win/history evidence; x86 paired-instance smoke. App-owned isolated history is required. |
| UAlbertaBot | `ualbertabot-sb-3` | Approve | Assigned Terran, fixed MarineRush configuration, ordinary local 1v1. No learning or arbitrary configuration support. One historical observer startup timeout remains recorded. |
| Marine Hell | `marine-hell-sb-4` | Approve | Assigned Terran, ordinary local 1v1, x64 Java 21. Corrected bunker adapter and installed-package launch verified. Unusual/island/no-choke maps remain unverified. |
| OpprimoBot | `opprimobot-sb-1` | Approve | Assigned Terran, ordinary local 1v1, fresh process each match. Natural defeats on both game architectures; caches/history stay disabled. |
| Infested Artosis | `infested-artosis-sb-1` | Approve experimental 1v1 | Assigned Zerg with x64 Java 21 and corrected morph/completion bridge. Keep `one-v-one: experimental`; late progression and one unclassified rejection remain outside verified scope. |
| Steamhammer | `steamhammer-sb-1` | Approve experimental 1v1 | Assigned Terran/Protoss/Zerg, ordinary local 1v1 with required frame-zero/mineral/refinery fixes. Keep experimental 1v1 and incompatible FFA. Final-build Terran x86 evidence is a concurrent smoke; its 1v1 combat evidence uses x64. |
| PurpleWave | `purplewave-sb-7` | Approve | Assigned Protoss/Terran/Zerg local 1v1. Metadata-only successor to reviewed sb.6; Java 21 x64 and required client fixes. Runtime bytes and evidence scope unchanged. |
| PurpleWave | Existing `purplewave-sb-1` through `sb-4` | Withhold | Earlier packages lack the libffi notice corrected in sb.4. sb.4 exposes three races while dedicated game evidence carried forward is Protoss-only. New sb.6 evidence does not approve these older bytes. |
| BananaBrain | `bananabrain-sb-5` | Approve experimental 1v1 | Assigned Protoss, Win32 native bot with x64 StarCraft, Fighting Spirit 1.4. Fresh package/source rebuild, natural passive-control win, isolated concurrency, interruption/save-failure and signed local install/reload/reset checks passed. Public tournaments require separate author permission. |

See [PurpleWave sb.7](purplewave-sb-7.md) for the exact parent/archive delta and
new immutable artifact.

Pluto remains development-only. No Pluto package, candidate, or approval is imported
by this change. Missing status remains a candidate; no other historical release is
implicitly approved.

## Evidence supporting the eight approvals

- **BananaBrain sb.5:** [exact admission review](https://github.com/coin-au-carre/robotics-facility/blob/62f7375ce8bdc16b4340896b9026fafd471577b0/docs/releases/bananabrain-sb-5.md)
  pins the fresh guarded build, offline extracted-source rebuild, original AI bytes,
  natural Protoss win at frame 8,683, two-instance gameplay, forced bot exit and
  zero-owned-process cleanup. Packaged-helper fault injection and replacement-boundary
  interruptions passed. Final sb.5 signed loopback installation rejected tampering;
  same-opponent reload retained prior history despite corrupt stale input. Active
  reset was rejected and idle offline reset preserved AI and package bytes. Keep
  experimental 1v1 and x64-game-only scope; two natural-match command rejection
  reasons, physical disk exhaustion, exhaustive tracing and active-human strength
  remain unverified. Review-only sb.2-sb.4 archives stay unapproved.

- **PurpleWave sb.7:** [metadata-successor review](purplewave-sb-7.md) pins the exact
  parent, changed fields, unchanged runtime JARs, inherited evidence limits, client
  dependencies, and remaining delivery steps.

- **ZZZKBot:** [release review](zzzkbot-sb-1.md) records pinned source/notice review,
  source-only rebuild, unsafe-name tests, a natural synchronized x64 win, history
  output and malformed-record play, separate x86 instances, owned-process cleanup,
  and signed staging readback. Arbitrary imported history and exhaustive hostile
  state/OS tracing are not covered. Profile isolation remains essential.
- **UAlbertaBot:** [source/release review](../ualbertabot-source-review.md) records
  corresponding-source rebuilding, fixed configuration, disabled StrategyIO/chat
  setters, x86/x64 gameplay and end callbacks, concurrent isolation, and unchanged
  configuration after play. The profile has no learning state to reload/reset.
  Its ordinary-map scope and recorded startup timeout remain explicit.
- **Marine Hell:** [admission](../marine-hell-admission.md) records the exact corrected
  JAR, notices and offline rebuild, x86/x64 progression/combat, actual Marines loaded
  into bunkers, signed CDN installation, installed practice launch, Java selection,
  and normal shutdown. The source has no persistent learning; forced crashes and
  unusual terrain were not exhaustively exercised.
- **OpprimoBot:** [admission](../opprimobot-admission.md) and
  [BWTA review](../opprimobot-bwta-review.md) cover source/dependency boundaries,
  corresponding-source rebuild, natural losses against ZZZKBot on x86/x64 with
  matching simulation probes and zero recorded command rejections, signed staging
  installation, paired launch, and cleanup. Fixed fresh-process configuration
  disables history/statistics/cache writes. Late-game spells/path timing remain
  broader follow-up work.
- **Infested Artosis:** [admission](../infested-artosis-admission.md) covers source and
  dependency notices, offline class reproduction, bounded history/isolation tests,
  corrected Zerg birth/completion behavior, a natural x64 loss, concurrent x86
  gameplay, and signed staging installation. Repeated installed launches retained
  earlier history rows; idle reset restored empty baselines. The earlier broken
  pre-fix run is excluded. These observations support experimental local 1v1
  availability, not an upgrade of its package's compatibility rating.
- **Steamhammer:** [admission](../steamhammer-admission.md) covers source/notices,
  offline native rebuild, storage regressions, final-build production/combat and
  normal concession callbacks, signed staging installation, repeated installed
  launches, and idle reset without package changes. Prior broken refinery/stale-DLL
  runs are excluded. Its recorded final endings were concessions, not natural
  combat results. The observed FFA Zerg stall remains a reason to keep FFA
  incompatible; it is not silently counted as successful Zerg/FFA gameplay.

These are artifact-specific reassessments of existing evidence. Limited command
rejections, missing exhaustive tracing, uncalibrated strength, and untested formats
are retained as limitations rather than relabelled as completed tests.

## PurpleWave successor and BananaBrain admission

[PurpleWave sb.4](purplewave-sb-4.md) fixes native libffi notice delivery but does
not supply dedicated Terran/Zerg gameplay for its all-race profile. sb.1-sb.3 do not
inherit that notice correction. The separately reviewed sb.6 runtime from
[PR #2](https://github.com/ShieldBattery/robotics-facility/pull/2) is now included here
as metadata-only [sb.7](purplewave-sb-7.md). Its original current-head fragment and
ZIP had a descriptor mismatch after project-attribution correction; the successor
resolves it with a new immutable identity and unchanged runtime/source/notices.
New runtime evidence is not backfilled into sb.1-sb.4, and the original sb.6 ZIP
remains intact. sb.7 needs its own prerelease upload and signed staging delivery.

[BananaBrain PR #3](https://github.com/ShieldBattery/robotics-facility/pull/3), reviewed
at `62f7375ce8bdc16b4340896b9026fafd471577b0`, now completes source/distribution
admission for exact sb.5. Its ZIP SHA-256 is
`c83f384e00d7c5ca61ccd5db5cc900d77379510cca47b775270773d71d245154`.
All 294 unchanged parent members include the tested runtime, source, original AI
and existing notices. Only descriptor/BUILD text changes and a current admission
notice is added. This catalog entry copies the final admitted descriptor verbatim;
only its external `releaseStatus` is added here. Public sb.5 delivery is still a
maintainer rollout step, not an already completed CDN test.

## Client and publication requirements

Use a ShieldBattery client with supervised release practice, signature/archive
verification, isolated per-instance writable profiles, lease-aware reset, and owned
process cleanup. [SB #1838](https://github.com/ShieldBattery/ShieldBattery/pull/1838)
supplies release practice and fixed production catalog trust. Approval is not a
claim that historical debug-only clients support production practice.

The tested native command/construction observations, Zerg morph/completion, and
frame-zero/mineral normalization fixes are in the current `feature/bwapi-compat`
line as `da532f3e2`, `11346b118`, and `e9abd018c`. Their older review commit IDs
were rewritten; this report does not infer compatibility from a stale binary.
[SB #1831](https://github.com/ShieldBattery/ShieldBattery/pull/1831) supplies accurate
collected-resource totals, and [SB #1841](https://github.com/ShieldBattery/ShieldBattery/pull/1841)
supplies building flight/Spider Mine translations where used. Ship the reviewed
client integration; these metadata approvals do not implement missing commands.

For the original catalog entries, only six external `releaseStatus` fields change.
The new sb.7 entry adds a metadata-only archive with unchanged runtime/source/configuration
and notices. BananaBrain adds the exact admitted sb.5 entry from PR #3. Historical
entries and archives stay intact. Fresh BananaBrain compilation/matches are recorded
in its admission; this approval PR does not rebuild them. Local publisher
preparation verified all 12 input releases and selected exactly eight approvals;
see [preparation evidence](local-practice-eight-validation.json). No public signing,
upload, workflow dispatch or production promotion is part of this change.

The existing signed staging revision does not gain these flags retroactively.
After review/merge, maintainers publish a **new signed staging revision** carrying
the approvals and exact existing archives, verify its activation and receipt, then
promote that revision through the protected production workflow. Select revision
numbers at publication time. Existing installations stay accessible under the
client policy; publication approval is not an installed-package revocation system.

## Verification of this metadata change

Readback verified the pinned staging signature and publication receipt for revision
12. Each of the six selected CDN archives passed the maintained archive verifier,
including its size, SHA-256, descriptor digest/content, paths, entry sizes, CRCs, and
required files. Their descriptors match the signed staging catalog and the source
catalog. The machine-readable record pins the verified signed envelope digest.

The initial six-release preparation retained exact staging archive and descriptor
bytes. The follow-up seven-release preparation includes the new locally verified
sb.7 archive. Its fresh public delivery is pending; sb.7 is not represented as an
already published staging artifact. Its destination was `.invalid`; nothing was signed
or uploaded. This dry run does not establish a newly signed staging receipt or
a production deployment. A new real staging revision remains required.

The current [eight-release follow-up](local-practice-eight-validation.json) also
verifies the exact local sb.5 archive and preserves all 11 prior catalog releases.
All 12 staging inputs pass verification; production selects exactly eight scoped
approvals. sb.5 and sb.7 still need public artifact delivery and a new signed
staging revision before promotion.
