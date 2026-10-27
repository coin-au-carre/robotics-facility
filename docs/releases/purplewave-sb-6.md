# PurpleWave sb.6 Gather guard review

Current production readiness is recorded in [the release-DLL follow-up](purplewave-sb-6-production.md).
This report preserves earlier validation and its then-open blockers.

Historical review-only candidate. See [admission follow-up](purplewave-sb-6-admission.md)
for completed source, Terran and installed/UI checks and newly identified bridge
blockers. Production approval remains withheld. The archive below is unchanged.

The candidate adds a hashed one-line Gather predicate guard to PurpleWave
`08672150433a2c60f5db23e10d2bc15dbc6e302e`, after its existing runtime isolation
patch. An empty melee-threat collection sums to zero and can satisfy the old
lethal predicate for a zero-health worker. Goal scoring then calls empty `max`.
The guard requires at least one eligible melee threat before entering that path.
Nonempty threat weighting and resource scoring are unchanged.

See [patch rationale](../../patches/purplewave/0002-guard-empty-gather-threats.md).
Historical logs lack health/threat telemetry; this explains a reproducible path,
not a reconstruction of those exact snapshots.

Validation: original compiled sb.5 predicate fails the zero-health/empty
threat assertion. The guarded Gather compiled against the same classes passes
eight regressions (health 0, -1, 1, 40; strict and lazy empty sequences). Locked
dependency install with scripts disabled, metadata generation check, typecheck,
lint/format, metadata/catalog validation and all 66 tooling tests pass. Fresh
full build, the same eight regressions against the full JAR, and review packaging
pass. Packaged source and guard patch bytes match their build inputs. Compared
with sb.5, only `Micro/Actions/Basic/Gather$.class` changes inside the bot JAR;
no entries are added or removed and all dependency JARs remain unchanged.

Recipe revision: `4bd56332018f3df71e47a3bbfdc83ce042e43c8f`.
Review archive SHA-256:
`c19bd7d3761621d221e652126208c7a1de52e8b5f15fd79e673cb8e5525875a1`.
Exact source tree, patch, JAR and recipe hashes are in
[pinned validation](purplewave-sb-6-validation.json).

## Gameplay

One fresh isolated local-build practice match completed naturally on x86, using
Fighting Spirit 1.4 and the exact sb.6 review archive/JAR. Protoss selected
`PvEProxy2Gate`; Zerg selected `ZvE4Pool`. Protoss won at frame 9,555 (14,515 accepted
commands, 36 rejected); Zerg lost at frame 9,551 (25,589 accepted, seven rejected).
Both saved-history winner/duration fields agree with native end callbacks.
Neither bot wrote a caught exception log; no `Gather.empty.max` occurred.
All test-owned app, game and JVM processes exited after cleanup.

Observed rejections hit target visibility, native actor/target availability,
ownership and target accessibility guards; no unsupported/malformed command
rejection appeared. This records rejection paths, not a reconstruction of every
native state transition. The match has different start positions and results from
prior sb.5 games; it is not a controlled playing-strength comparison or proof that
the rare zero-health/empty-threat snapshot occurred. The compiled regression
provides deterministic coverage for that predicate boundary.

SB source was diagnostic commit `fc315a61f9bd175eaf34a9284bfd96e4224a5c98`,
including gathered-resource and train-cancel fixes. Loaded x86 DLL SHA-256:
`b4800e8f4dbce8e0080be920c79d5e74fca74880d2130238178e35f2a3f78360`.
The bot uses x64 Java 21 even though the tested game was x86.

## x64 match and later saved-state reload

The same immutable sb.6 archive completed a fresh natural x64 match on Fighting
Spirit 1.4, again using `PvEProxy2Gate` against `ZvE4Pool`. Protoss lost at frame
8,962 (11,785 accepted commands, seven rejected); Zerg won at 8,966 (41,772
accepted, 32 rejected). Both saved-history winner/duration fields match native
callbacks. Neither bot wrote a caught exception log. Rejections hit native
actor/target availability, ownership and visibility guards; none were
unsupported/malformed commands.

The x64 DLL SHA-256 was
`2dcb04a73fead4c0804e9ca5d6d3bd9f78974f53432f66105e1429b4e0b7dedc`,
from the same diagnostic SB commit recorded above. The full-match process exited
before the next run. First-match history files were preserved in private snapshots.

For the later reload, each genuine first-match history file was copied unchanged
into that bot's `read` directory while retaining its identical `write` copy.
The same owned profiles and exact bot JAR restarted. No rows were invented or
edited. Per-bot JFR recordings started with each JVM, through test-only arguments
on the local-build entries; the review archive and its runtime metadata were not
modified.

JFR records one positive history-file read from `read`, followed by one from
`write`, in each JVM: 175 bytes per copy for Protoss and 123 for Zerg. Read-input
SHA-256 values stayed unchanged, and no JFR file-write event targeted those read
inputs. After the later run, each output contained exactly two records: its
original genuine row exactly once and one new actual result. This verifies later
reload and deduplication of the identical read/write rows. Source inspection of
`HistoryLoader` confirms first-seen timestamp precedence with `read` before
`write`; conflicting-row precedence was not separately fault-injected.

Reload gameplay was deliberately bounded at frame 1,500, then stopped through
`localGameStop`. Native callbacks occurred at frames 1,552 (Protoss) and 1,553
(Zerg), both reporting loss after manual stop; the new history winner/duration
fields agree. No command rejections or caught bot exceptions occurred. This is
bounded reload and graceful-stop evidence. The preceding x64 match supplies the
natural-completion evidence. All owned test processes exited.

Captured JFR reader stacks contain `FileInputStream`/`StreamDecoder` frames but
omit the `HistoryLoader` frame. Exact profile paths, read order, unchanged input
hashes, source inspection and retained output rows provide complementary reload
evidence; full call-stack attribution is not claimed. Raw JFR/decoded paths stay
private. Exact measurements and hashes are under `followUp` in the
[machine-readable validation](purplewave-sb-6-validation.json).

sb.6 Terran, Random, other formats, signed-catalog installation and full native
runtime tracing remain unverified. Existing sb.5
source/runtime and gameplay evidence remains scoped to sb.5. Source/distribution
admission remains pending. The review archive retains its conservatively unverified
profile metadata from packaging; later candidate evidence does not rewrite it.
Existing catalog, sb.5 ZIP and pinned working bots are unchanged.
