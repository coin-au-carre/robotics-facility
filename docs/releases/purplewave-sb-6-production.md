# PurpleWave sb.6 production readiness

Approve exact sb.6 archive for local 1v1 with assigned Protoss, Terran or Zerg on
ShieldBattery clients containing the required bridge/runtime fixes. The proposed
catalog sets `releaseStatus: "production-approved"` for this release only. Existing
sb.4 and other releases gain no approval from this review.

This is a reviewed publication proposal, not a live catalog change or deployment.
[Catalog fragment](purplewave-sb-6-production-catalog.json) contains exact sb.6
bytes and approval metadata for merging into the current catalog. This PR leaves
the live catalog and every existing bot/release unchanged. [Machine-readable evidence](purplewave-sb-6-production-validation.json)
records hashes, per-instance observations, native outcomes and limitations.

## Exact reviewed bytes

- Archive SHA-256: `60f80501b9d898b0b392df624e35d15df4bcb3749338e77b8e78820405e980b4`.
- PurpleWave JAR SHA-256: `e2bfd2519e07e6597bd2159b6044df6c2fca11ebc2b86a532f28e182bd452013`.
- Original review archive, source pin, offline reproducible rebuild, runtime review,
  permissions and notices remain recorded in the [admission report](purplewave-sb-6-admission.md).
  The admitted ZIP differs from the original review ZIP only in its package descriptor.
- Bot source, JAR, runtime configuration and installed package files were unchanged
  during these checks. Only isolated writable profiles received strategy selections.

## Fixed blockers and release verification

[ShieldBattery #1841](https://github.com/ShieldBattery/ShieldBattery/pull/1841)
translates Lift, Land and Vulture Spider Mines through native synchronized commands.
[ShieldBattery #1838](https://github.com/ShieldBattery/ShieldBattery/pull/1838)
activates the release bridge only for supervised local bot instances and resolves
production practice through explicit catalog approval. Direct raw launches,
legacy discovery and debug controls remain development-only.

Both DLL architectures built through normal `build.bat release`, with dependency
and font license notices. Signed isolated HTTPS installation, invalid-signature
rejection, archive-tamper rejection, installed launch, profile reset and unchanged
package checks passed on both architectures. Local signing tests used development
trust configuration; production keys and public CDN delivery were not exercised.
The actual production Electron IPC path separately rejected raw launches and
reached installed-bot validation for practice. Earlier real UI reset checks remain
in the admission report; these release runs exercised reset IPC.

Two installed Terran instances per architecture played `TvE3Fac` beyond frame
15,000, followed by a graceful manual stop. Native snapshots confirmed lifted
Barracks, Vultures, Siege Tanks and actual Spider Mine units. Peak simultaneous
mine counts were 34/39 on x64 and 54/33 on x86. These are bounded progression
checks, not natural results. All 63 common simulation probes through frame 14,880
matched across the three clients in each architecture's factory game.

Separate minimal JBWAPI fixtures harvested resources, constructed Depots/Barracks,
lifted and landed those buildings. Both instances completed both transitions on
each architecture. These establish bridge behavior, not PurpleWave strategy or a
natural result.

## Natural results and saved-state agreement

Each listed result completed before observer cleanup. Actual history CSV rows
agree with the native MatchEnd winner flags; every match has one win and one loss.
CSV hashes and native accepted/rejected counts are recorded in the JSON report.

| Architecture | Assigned races | Native result | End frames |
| --- | --- | --- | --- |
| x64 | Terran / Zerg | Terran loss, Zerg win | 4,870 / 4,875 |
| x64 | Protoss / Zerg | Protoss win, Zerg loss | 9,370 / 9,365 |
| x86 | Terran / Zerg | Terran loss, Zerg win | 5,428 / 5,433 |
| x86 | Protoss / Zerg | Protoss loss, Zerg win | 6,203 / 6,208 |

An earlier x64 Terran/Zerg natural run ended at 9,055/9,060. Its watcher stopped
on a partially written progress JSON, so CSV hashes survived but row bytes did
not. That run is not used for saved-row reconciliation. A fresh natural run
retained real CSV bytes synchronously and supplies the x64 Terran row above.
Later event/poll captures preserved actual files before resets. No rows were
reconstructed. Prior saved-history reload, precedence, corruption, reset and
source/runtime tracing evidence remains scoped to the unchanged sb.6 bot bytes.

Factory native guard totals were 108/51 rejected commands on x64 and 47/43 on
x86. No unsupported translation warnings appeared. Existing logging suppresses
those warnings once total rejections reach ten, so this is not a zero-rejection
or complete-classification claim. Earlier full traces identified Lift/Spider
Mines plus stale native actor/target guards. Actual lift/landing/mine behavior is
independently observed here. No bot exception log was observed in the retained
release-run inventories.

## Required rollout and remaining scope

Integrate ShieldBattery [#1831](https://github.com/ShieldBattery/ShieldBattery/pull/1831)
(gathered resources), [#1837](https://github.com/ShieldBattery/ShieldBattery/pull/1837)
(training cancellation), [#1838](https://github.com/ShieldBattery/ShieldBattery/pull/1838)
(approval and release runtime), and [#1841](https://github.com/ShieldBattery/ShieldBattery/pull/1841)
(flight/mines), plus robotics-facility [#4](https://github.com/ShieldBattery/robotics-facility/pull/4)
(approval schema/publisher). Older clients/bridges are not covered by this approval.

Follow the maintained [publication workflow](../github-actions.md): upload exact
reviewed immutable ZIP as a versioned prerelease, verify its digest, reconcile the
reviewed catalog proposal into the current catalog, then publish a new signed
staging revision. Verify its actual CDN bytes and receipt before production
promotion. Choose the next staging revision at publication time. No upload,
workflow dispatch or production promotion was performed during this review.

Random race, teams, FFA, unusual maps and exhaustive late-game command conformance
remain unverified. Human difficulty remains uncalibrated; public competition
permission remains unreviewed. Source/runtime review is not an OS sandbox. The
legacy `compatibility.status` stays `prototype-tested` because its schema offers
only that positive value; release approval is the separate explicit catalog field.

## Contributor checks

Both integration targets passed 640 Rust tests, full normal all-target/workspace
Clippy with warnings denied, release library Clippy and formatting. Full TypeScript,
161 bot/practice tests, focused app lint/format, production Electron build and
actual production IPC smoke checks passed. Robotics metadata/schema, typecheck,
lint/format, repository validation and all 66 tooling tests passed. The proposed
catalog fragment and its seven-bot merge validated against approval schema #4; a local publisher dry run
verified the exact archive for staging and production preparation with injected
disk bytes and `.invalid` URLs, without network, signing or upload.

All 33 test-owned game IDs and three Electron owners were checked after cleanup;
no owned Electron, SC:R, Java or evidence-watcher process remained. The isolated command change separately
passed both debug DLL builds, 634 tests per target and full normal Clippy.

An extra release-unit-test attempt hit 20 pre-existing compilation errors in
unchanged upstream debug-control tests in `app_socket.rs` and `netcode_v2/mod.rs`.
Release DLL builds and release library Clippy passed; debug controls stayed out of
release DLLs. This extra suite is not reported as passing.
