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
full build and review packaging are in progress.

No sb.6 gameplay, natural match completion, saved-state
reload, installation or full runtime tracing is claimed. Previous sb.5 evidence
remains scoped to the original archive. Existing catalog and sb.5 ZIP are unchanged.
