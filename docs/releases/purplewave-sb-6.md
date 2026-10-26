# PurpleWave sb.6 Gather guard review

Review-only candidate. Source and distribution admission remain pending.

The candidate adds a hashed one-line Gather predicate guard to PurpleWave
`08672150433a2c60f5db23e10d2bc15dbc6e302e`, after its existing runtime isolation
patch. An empty melee-threat collection sums to zero and can satisfy the old
lethal predicate for a zero-health worker. Goal scoring then calls empty `max`.
The guard requires at least one eligible melee threat before entering that path.
Nonempty threat weighting and resource scoring are unchanged.

See [patch rationale](../../patches/purplewave/0002-guard-empty-gather-threats.md).
Historical logs lack health/threat telemetry; this explains a reproducible path,
not a reconstruction of those exact snapshots.

Validation so far: original compiled sb.5 predicate fails the zero-health/empty
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

sb.6 x64, Terran, Random, other formats, later saved-state reload, signed-catalog
installation and full native runtime tracing remain unverified. Existing sb.5
source/runtime and gameplay evidence remains scoped to sb.5. Source/distribution
admission remains pending. The review archive retains its conservatively unverified
profile metadata from packaging; later candidate evidence does not rewrite it.
Existing catalog, sb.5 ZIP and pinned working bots are unchanged.
