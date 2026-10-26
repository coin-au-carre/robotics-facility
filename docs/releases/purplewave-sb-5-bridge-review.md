# PurpleWave sb.5 bridge and native review follow-up

Automated review, 2026-10-26. Admission remains pending. This report extends
[prior validation](purplewave-sb-5.md); it does not change the reviewed ZIP,
source pins, executable JARs or published catalog. Sanitized measurements are in
[purplewave-sb-5-bridge-review.json](purplewave-sb-5-bridge-review.json).

## Missing training cancellation

Opt-in rejection diagnostics found seven unsupported commands in a natural
Protoss-versus-Zerg x64 match: BWAPI `Cancel_Train` (kind 35), default slot `-2`.
The bridge lacked this encoding. Other rejections hit actor/target availability,
ownership or visibility guards. Earlier uninstrumented match counts cannot be
classified retroactively.

The separate SB fix translates kind 35 and `Cancel_Train_Slot` (kind 36) to
native opcode `0x20`, signed slot byte and a zero padding byte. Default cancellation
encodes `[0x20, 0xfe, 0]`; explicit slots 0 through 4 encode `[0x20, slot, 0]`.
Unexpected slots, targets and coordinates are rejected. Existing native ownership
and visibility guards remain in force.

The packet layout was checked against pinned BWAPI
`7687da8abc4726f8366401f11ab648d421385793`: `BW/OrderTypes.h`'s
`Orders::CancelTrain` and `BWAPI/GameCommands.cpp`'s two cancellation cases,
plus `BWAPILIB/UnitCommand.cpp`'s default `-2` selection. Two tests cover exact
selection/packet bytes, every explicit queue slot and malformed arguments.

[Review patch](purplewave-sb-5-cancel-train.patch) is the complete 57-line addition
to `game/src/bwapi/commands.rs`, based on freshly rebased
`feature/bwapi-compat` at `a096b05e199ac0aaa6d46d7da21b8833acfd3218`.
Local fix commit: `e09aa8c28bc53172b2006fbce8d52734ad601269`.
This is an application fix, separate from the bot recipe. Fresh-base `build.bat debug` and `build.bat debug x86` both pass. All-target
tests pass with 634 tests per architecture; full workspace/all-target Clippy with
`-D warnings` and `cargo fmt --all -- --check` pass. DLL hashes and exact commands
are recorded in the machine-readable follow-up. Metadata/catalog checks pass;
the review archive SHA-256 remains unchanged.

## Live comparison

All runs used the identical sb.5 review archive, Fighting Spirit 1.4, separate
profiles and the existing local-build/practice flow. Fixed runs selected
`PvEProxy2Gate` versus `ZvE4Pool`. Baseline Protoss selected the same opening.
Start positions varied, so these counts do not compare playing strength or speed.
Both game callbacks and saved history agree in every listed run. All matches
finished naturally before observer cleanup or frame limits.

| Build / game architecture | Protoss result and commands | Zerg result and commands | Accepted default training cancels |
| --- | --- | --- | --- |
| Before fix, x64 | Loss at 7,133; 6,202 accepted / 11 rejected | Win at 7,138; 25,580 / 56 | 0; seven unsupported cancellations |
| Fixed, x64 | Loss at 7,536; 6,045 / 2 | Win at 7,541; 35,968 / 36 | 4 |
| Fixed, x86 | Loss at 6,668; 5,408 / 6 | Win at 6,673; 20,890 / 39 | 5 |

Fixed runs had no unsupported/malformed command rejection. Remaining reasons:

- x64 Protoss: one missing native actor, one invisible target. Zerg: 34 missing
  native targets, one native ownership failure, one invisible target.
- x86 Protoss: five missing native actors, one missing native target. Zerg: 38
  missing native targets, one native ownership failure.

These guards can reject commands based on stale snapshots. Logs identify each
rejection path, but do not fully reconstruct underlying native lifetime/ownership
transitions. Live coverage exercises default cancellation; explicit queue slots
have encoder unit coverage only.

Caught `UnsupportedOperationException: empty.max` in PurpleWave's
`Micro/Actions/Basic/Gather.scala:98` occurred four times before the fix and once
in fixed x86. The expression takes `max` over filtered melee threats. Bot's frame
exception handler continued and both matches saved their actual final result.
Fixed x64 had no such error. Underlying bot-state cause remains unresolved;
this is not a bridge command-encoding exception or an architecture diagnosis.
Checking stdout alone would miss these profile error logs.

Live fixed DLLs came from diagnostic source
`fc315a61f9bd175eaf34a9284bfd96e4224a5c98`, based on gathered-resource fix
`ff167db43053cead7a991a68c224c8d6bf61066d` (SB PR #1831):

- x64 SHA-256: `2dcb04a73fead4c0804e9ca5d6d3bd9f78974f53432f66105e1429b4e0b7dedc`.
- x86 SHA-256: `b4800e8f4dbce8e0080be920c79d5e74fca74880d2130238178e35f2a3f78360`.

[Opt-in diagnostic patch](purplewave-sb-5-rejection-diagnostics.patch) reproduces
per-reason logs and accepted train-cancel counts. Apply it and the train-cancel
patch to `ff167db`; set `SB_BWAPI_REJECTION_DIAGNOSTICS=1` in the test app
environment. It changes only `bwapi.rs` and is excluded from the production fix.

Hashes were recorded before fixed tests. The pre-fix diagnostic DLL hash was not
retained; its source commit was `7a13c9dad681cb62f1f42323f71d88ccbdca0664`.
Diagnostic and fresh-base production `commands.rs` have identical Git blob
`43127cbd2feb52839520f1d673824f1fd33d38d0`. Live tests used diagnostic DLLs,
not the separately rebuilt fresh-base production DLLs.

## Native runtime tracing

A user-approved WPR file-mode capture enabled kernel process/thread, loader,
file I/O/name, registry and network tracing. Only the recorder used elevation;
tested Electron/JVM ran without elevation. The bounded Protoss x64 run started
at 13:40:35.276 UTC and ended at 13:41:19.661 UTC. Native file events cover JVM
startup through shutdown, 13:40:37.187–13:41:17.133 UTC. This run used the prior
verified gathered-resource DLLs and identical sb.5 archive, not the later cancel
fix. A preceding memory-mode recording retained only its circular tail and does
not establish startup coverage.

Native ETL decoding used Windows SDK `ProcessTrace` and TDH, retaining events
whose header process ID matched the test JVM. Of 18,625,436 total records,
14,451 matched; 13,999 decoded and 452 lacked a decoded schema. No lost events
or buffers were reported. Decoded task totals: 12,757 file I/O, 1,021 registry,
112 image, 106 thread and three process events. Raw ETL and decoded paths remain
private because the system recorder also contains unrelated process activity.

File opens covered package/JRE files, Windows runtime configuration and DLLs,
assigned BWAPI IPC, isolated bot profile and JNA extraction under test TEMP.
All 336 attributed write events correlated by file-object lifetime to six files
inside bot profile or isolated TEMP. No attributed package/JRE/game write appeared.
This does not claim that every possible filesystem operation was observed.

Registry events primarily queried Windows/JVM settings. One set-value event
named `conhost.exe` occurred at launch; nine set-information events had empty
key names. Complete call-stack attribution was not retained. These events remain
explicit review limits, not a claim of zero registry writes.

The JVM also read 90 bytes from an existing Windows CryptoAPI container, with no
writes to it. [OpenJDK's Windows seed generator](https://github.com/openjdk/jdk21u/blob/master/src/java.base/windows/native/libjava/WinCAPISeedGenerator.c)
uses named `J2SE` container with `CryptGenRandom`; inspected source blob was
`e447ecc43a90119ef4beaa7cc157ced3bad7f852`. A read-only metadata check opened
that existing context with flags zero and compared `PP_UNIQUE_CONTAINER` with
the observed filename privately: exact match. No container was created; no keys
were enumerated or exported. User SID/container filenames are omitted. This
supports standard Java RNG initialization as the explanation for that access.

No JVM-header network event appeared in this capture, and sampled TCP/UDP
endpoints were zero. Observed child was `conhost.exe`. Header-PID filtering can
miss deferred kernel network work; prior JFR/source review supplies complementary
evidence, not proof of complete network exclusion. This capture covers one bounded
Protoss x64 run, not all races, configurations, failure paths or an OS sandbox.

## Remaining admission work

Source/distribution admission still requires reviewer decision on exact archive
and documented limits. Native attribution is incomplete for 452 undecoded events
and registry changes. The caught bot `Gather` error remains open. Terran full
matches/x86, Random, other formats, signed-catalog installation and installer/UI
reset remain unverified. The separate gathered-resource and train-cancel bridge
fixes must be available in the app used for deployment. No publication is implied
by these results or candidate metadata.
