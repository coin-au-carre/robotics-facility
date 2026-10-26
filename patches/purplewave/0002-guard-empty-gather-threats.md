# Gather empty melee-threat guard

Pinned source: `08672150433a2c60f5db23e10d2bc15dbc6e302e`, after runtime isolation
patch. Modified by ShieldBattery, 2026-10-26. Upstream submission: not submitted.
PurpleWave remains MIT; original notices and author attribution are preserved.

`stabbingLethal` sums predicted melee damage, then compares with total health.
An empty collection sums to zero, so a zero-health worker passes the old predicate.
With an enemy range tile, a base and an eligible resource, goal scoring then calls
`max` on that empty collection and throws. The patch requires `stabbers.nonEmpty`
before calculating lethal damage. Nonempty damage weighting and goal scoring are
unchanged. It does not add a fallback resource or change ordinary gathering.

Regression invokes the compiled Gather predicate for health 0, -1, 1 and 40,
using both strict and lazy empty collections. Original sb.5 code fails the
zero-health assertion; patched code must pass all eight cases. The harness uses
synthetic snapshots and reflection over Scala 2.12's generated local lazy method.
It isolates the predicate and does not simulate an entire match.

Prior matches logged `empty.max` at Gather.scala:98 without health/threat telemetry.
The empty-list/zero-health path is reproducible; exact historical snapshot values
are inferred, not recovered. Source tracks `alive` separately from health and
reads friendly health from BWAPI. This patch does not diagnose why a dying worker
snapshot reached micro or claim that ShieldBattery produced an invalid snapshot.

New source inputs require a fresh review package and admission decision. sb.5
archive, historical results, published catalog and pinned working bots stay intact.
