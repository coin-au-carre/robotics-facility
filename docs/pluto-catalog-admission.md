# Pluto catalog integration review

Pluto uses an adapter-only facility archive and SB `pluto-v2` catalog runtime.
Original model files are fetched directly from pinned official release during
installation. This avoids rehosting the 295 MiB upstream archive or claiming a
redistribution grant for closed original binaries.

Facility package contains exact source-built .10 companion and complete compiled
source/dependency inventory, original MIT/LGPL notices, modification disclosure and
upstream pin document. Companion source snapshot is pinned at facility commit
`8cd1f60680efcb81ac92cd7221d4ad45b4ed5606`. Historical model/companion operands
and earlier evidence stay unchanged.

## Installation contract

Signed catalog/package identifies companion digest and x86 launch. Installer checks
package descriptor against signed metadata before downloading originals. Embedded
upstream JSON must equal SB's supported v2.0 pin. Original ZIP and each model file
are SHA-verified in staging; promotion occurs only when complete. Failure/cancel
removes staging/downloads and preserves previous installed release.

Normal catalog selection launches through existing in-process companion host,
without local-build import or external user directory. Assets seed isolated working
copies, learning remains per-instance, and reset clears only read/write baselines.
Human/observer keeps configured architecture; only Pluto uses x86. Helper lifetime
is tied to game job; this is not an OS sandbox.

## Review scope

Prior evidence includes finite raw-memory, metadata, terrain, neutral cache, scalar,
neural/action-history, full synthetic startup and whole-binary inventory reviews.
`.10` tested historical artifact completed a natural Terran match. A separately
source-built artifact from exact source passed official-download/local-library
launch with real commands and graceful cleanup. Those are not reassigned as signed
catalog install evidence or complete fog certification.

Current catalog install/play/reload/reset evidence is in progress. Candidate source
review remains pending until exact shared-package path is verified. Local
distribution approval covers adapter/source archive and notices, not redistribution
of original closed models. Full binary noninterference, all map/race combinations,
arbitrary hostile state and OS containment remain outside approval scope.

Production rollout additionally requires SB host/installer release, public adapter
artifact/source availability, approved catalog metadata and existing maintainer
staging/production signing workflow. Local tests do not activate public catalog.
