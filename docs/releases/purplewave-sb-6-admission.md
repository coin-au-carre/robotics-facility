# PurpleWave sb.6 experimental staging admission

sb.6 remains **experimental local 1v1**, with assigned Protoss, Terran or Zerg.
Source/local-distribution review is approved for the pinned configuration and
recorded source/runtime boundaries. **Production approval is deferred.** Public
competition, human difficulty, Random, teams, FFA, unusual maps and exhaustive
late-game command conformance remain unverified.

## Corrected unpublished package

The prior local installer archive and edited production fragment no longer agreed:
the fragment changed downstream attribution while retaining the old descriptor and
ZIP digests. The corrected archive is built and packaged through maintained guarded
tooling from committed recipe `17b377ca0d5df068e52abb5bdcf57867d697477f`.
It records `ShieldBattery` as downstream modifier and experimental 1v1 in both
embedded descriptor and [staging fragment](purplewave-sb-6-staging-catalog.json).
The archive verifier checks descriptor equality, digest, entry sizes, CRCs, paths
and required files.

- Corrected ZIP SHA-256: `477ee3cc900eb0851bcd726c8e2f8781579a38daaf8c0f876383b5127f0db722`.
- ZIP size: 20,065,830 bytes; 1,186 unique entries.
- Embedded descriptor SHA-256: `18362afc51831f3bd78135edbf42bcbee4fec5960715811ecce4f2e5cef1d6f0`.
- Bot JAR SHA-256: `e2bfd2519e07e6597bd2159b6044df6c2fca11ebc2b86a532f28e182bd452013`.

All six runtime JARs, patched upstream source, notices and working configuration
are byte-identical to the prior admitted archive
`60f80501b9d898b0b392df624e35d15df4bcb3749338e77b8e78820405e980b4`.
Only `package.json`, `source/build-info.json` and
`source/tools/package-purplewave.ts` change. No members are added or removed.
Original local archives remain preserved by their original digests; no published
archive or historical catalog descriptor is overwritten.

The sb.6 release endpoint returned 404 during reconciliation, and the public release
inventory contained PurpleWave sb.1-sb.4 only. The retained sb.6 ID identifies a
corrected **unpublished** candidate, not a replacement for an immutable published
release. Upload exact reviewed bytes before adding its fragment to active catalog.
The current `catalog/catalog.json` remains unchanged while that URL is unavailable.

Fresh checks are recorded in
[pinned staging validation](purplewave-sb-6-staging-validation.json). Earlier
measurements are preserved separately in
[historical evidence](purplewave-sb-6-history.json), with their original archive,
client, architecture and toolchain scope. They are not new tests of this ZIP.

## Player-facing update

The source moves from `a57d2511cc4f6318c2a2d61a504e8f58a7b090af` to
`08672150433a2c60f5db23e10d2bc15dbc6e302e`, a 172-commit upstream update.
It includes PvP opening/defense changes, Dark Templar targeting, Observer survival
and scouting fixes, and pathfinding/combat-simulation performance work. These are
source changes, not a measured playing-strength or performance improvement for this
package. The local Gather guard prevents an empty-threat scoring exception for
zero-health worker snapshots; nonempty threat weighting and goal scoring stay
unchanged. [Patch rationale](../../patches/purplewave/0002-guard-empty-gather-threats.md)
records the compiled boundary case.

## Source, runtime effects and distribution

Both hashed patches apply to the exact source pin: runtime/storage isolation and
the nonempty-threat Gather guard. The review covers the full upstream delta,
patched source and fixed configuration, dependencies, build tools and delivered
notices. Upstream head after the selected pin contained unresolved Aim references;
this package does not include speculative combat workarounds.

The update adds fixed `bwapi-data/AI/fixedbuilds-by-opponent.json` input, with
opponent names selecting JSON keys rather than constructing paths; fallback remains
fixed `fixedbuilds.txt`. Placement-cache format changes from 3 to 5 and old caches
are regenerated. Opponent-history format v5 and its read/write deduplication remain
compatible. Source/runtime evidence for these paths is retained at original scope
in the historical record.

Fixed configuration disables human, ladder, livestream, tournament, pretraining,
debug and visualization modes. External visualizer auto-launch remains removed.
Active effects are package/config reads, assigned BWAPI shared-memory/named-pipe
IPC, encoded opponent histories, versioned placement caches and logs below isolated
app-owned profiles. Optional mjson URL/schema APIs and standalone research entrypoints
are not invoked by packaged main path. Gather guard adds no file, network or
process operation. Prior scoped WPR/JFR observations retain their attribution and
coverage limits; this is neither an OS sandbox nor a complete syscall trace.

Package preserves original PurpleWave MIT notice and all component notices,
including Java BWEM, JBWEB, JavaJPS, mjson, Scala, JNA/JNA Platform, Commons Lang and
native libffi. Corresponding patched sources, locks, patches and offline build
materials are included. Java is detected separately and is not bundled. The source
build invokes pinned Java/Scala compilers directly, without upstream Maven plugins,
Python helpers or launch scripts. Original author attribution remains intact;
source/local redistribution approval grants no public-competition endorsement.

## Historical gameplay and client requirements

Recorded runtime tests used the byte-identical six-JAR runtime with the exact
clients/DLLs in historical evidence. They include natural Protoss/Zerg and
Terran/Zerg outcomes on x86/x64, winner/frame agreement in real saved rows, later
history reload/deduplication, isolated simultaneous profiles and owned-process
cleanup. Bounded factory games observed lifted Barracks, Vultures, Siege Tanks
and actual Spider Mine units; separate fixtures exercised Lift and Land. Bounded
stops, manual concessions and fixture behavior are not natural full-match results.

Earlier clients rejected Lift and Spider Mines. Separate client fixes corrected
those paths in recorded release-DLL integration; those successful observations do
not establish support in every shipped client. Rejection counts and limitations
remain in original records. The corrected archive does not acquire a broader
verification claim from unchanged JARs.

Required client behavior includes gathered-resource reporting, training cancellation,
building-flight/Spider Mine translation, supervised release practice, signature and
archive verification, isolated writable profiles and lease-aware reset. The current
`minimumBridgeVersion` field is descriptive and is not enforced by the client;
bumping its string alone would not gate installation on these fixes.

## Staging and production rollout

No production approval field or standalone production proposal remains. Candidate
and package keep experimental 1v1. The fragment has no `releaseStatus`; the separate
production gate excludes such entries before download. Production approval needs a
later reviewed change after required client rollout and exact signed staging
artifact delivery are verified.

Experimental catalog admission also requires the per-release production gate from
[PR #4](https://github.com/ShieldBattery/robotics-facility/pull/4). A publisher without
that gate must not promote this candidate merely because source review is approved.
After gate integration, maintainers host this exact ZIP at its versioned artifact
URL, then merge the matching experimental fragment and publish a new signed
staging revision through existing workflow. Availability is not inferred from this
local package. No release upload, catalog signing with production keys, workflow
dispatch, public delivery or production promotion is part of this reconciliation.
