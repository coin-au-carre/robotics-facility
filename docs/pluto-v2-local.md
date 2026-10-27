# Play Pluto v2 in ShieldBattery

This bundle supplies Pluto v2 with ShieldBattery companion guard `.10`. It uses
original model DLL, inference helper and weights. Select Terran, Protoss, Zerg or
Random in a two-player Melee game on a map at most 128 by 128 tiles. An observer
can watch two bots play. Windows x64 with AVX2 is required; Pluto's game runs x86.

## Add to your library

1. Extract the ZIP into a folder you keep. Keep `bundle` and its files together.
2. Open ShieldBattery development build containing companion host support.
3. Open **Practice → Bots → Use a local build**.
4. Select `bundle/pluto.dll`. Adjacent `sb-bot.json` supplies name and settings.
5. Choose Pluto v2 as opponent and launch a two-player **Melee** game.

ShieldBattery copies runtime into app-owned working directories and keeps learning
there. Original extracted bundle stays unchanged. Each simultaneous instance gets
its own working copy. Use library's reset-learning action while bot is idle to
start over. Do not move individual model files or mix releases.

Current companion host exists on local `prototype/pluto-v2-companion` branch. It is
not yet included in official ShieldBattery releases. Use development/debug game
build; all BWAPI practice bots on that branch use experimental bridge support.
This bundle is imported locally, not downloaded from signed shared catalog.

## Ownership and rebuilding

Robotics-facility owns adapter source, pinned input manifest, local packaging recipe
and bot bundle. ShieldBattery owns installation, library selection and minimal
in-process host/lifecycle support. Existing external BWAPI native host cannot run
this legacy-memory adapter without a different adapter implementation.

Matching companion source and dependency notices accompany this bundle in `source`.
From Windows Visual Studio environment, rebuild companion with:

```powershell
cmake -S source -B rebuild -G "Visual Studio 17 2022" -A Win32
cmake --build rebuild --config Release --target pluto-companion
```

Rebuilt binary may have a different digest. Register its actual digest in a separate
local-build descriptor; never bypass hash verification or overwrite pinned original
model operands. Source here covers companion and dependencies, not closed Pluto
model binaries.

From robotics-facility checkout, prepare a fresh local bundle from user-supplied,
hash-matching `.10` files using Node 24 and installed workspace dependencies:

```powershell
node tools/package-pluto-v2-local.ts C:\path\to\guard-10-bundle pluto-v2-local-new
```

Output: `dist/local/pluto-v2-local-new/`, containing ready-to-import bundle, matching
source, manifest, this guide and ZIP. Source lives in `native/pluto-v2-companion`;
`native/pluto-v2-inputs.json` pins all 205 compilation/dependency/test source files.
Packaging preserves tested companion bytes rather than silently rebuilding them.

## Tested scope

Exact `.10` artifact completed a natural Terran match against Protoss BananaBrain
on Fighting Spirit 1.4. Earlier guards completed all three races; those outcomes do
not certify this artifact across every matchup. Startup and synthetic fog/history
reviews provide additional finite evidence. Original model operands are unchanged.

Bot is experimental. Full fog safety, arbitrary malformed state, every map/race
matchup and whole-binary isolation are not certified. Job ownership cleans up helper
lifetime; it is not an OS security sandbox. Existing review limitations stay in
facility review documents. No measured human difficulty is assigned.

Local use of user-supplied binaries is separate from public redistribution. Shared
catalog publication still needs recorded release distribution terms and package/
host integration. This local archive changes no catalog approval fields.
