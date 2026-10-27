# Pluto v2 catalog package

Requires Node 24, Windows MSVC Win32 and CMake. Build adapter from facility-owned
sources and pinned official inputs, then prepare a fresh review release:

```powershell
node tools/prepare-pluto-v2-local.ts pluto-catalog-build-new
node tools/package-pluto-v2.ts .build/pluto-catalog-build-new-prepare pluto-sb-new --review
```

Catalog archive contains companion, matching source and dependency notices. Original
Pluto DLL/helper/weights are not included. SB's `pluto-v2` runtime downloads official
v2.0 archive, checks its pinned size/SHA and extracted-file hashes, then promotes
complete package atomically. Signed archive includes `pluto-upstream.json`; client
requires it to match supported model release exactly. Large originals stay outside Git.

SB forces Pluto game client to x86, retains normal architecture for human/observer,
and uses same guarded companion ABI as local builds. AVX2 and Windows x64 required.
Only 1v1 Melee on maps up to 128 by 128 tiles is supported.

Adapter source is pinned through `pluto-companion` source-lock entry. Existing
BWAPI pin and retained notices document dependency. Offline companion rebuild:

```powershell
cmake -S source/companion -B rebuild -G "Visual Studio 17 2022" -A Win32
cmake --build rebuild --config Release --target pluto-companion
```

Review packaging does not publish or approve catalog. Maintainer publication runs
through protected main staging/production workflows after exact release evidence
and approval. Existing user learning and older tested bundles stay unchanged.
