# BananaBrain native host prototype

Local prototype only. No catalog entry or distribution approval. Uses unmodified
AIIDE 2025 source and the existing native host/BWAPI client; no custom companion.

The verified archive and local source-import branch are recorded in
imports/bananabrain-aiide2025.json. Source/ and BWEM compilation units mirror the
original Visual Studio project. Original license notices remain in source.
The author license has a separate public-tournament permission condition.

Build committed recipe inputs on Windows with Node 24, CMake and Visual Studio:

```powershell
node tools/build-bananabrain.ts bananabrain-prototype
```

For an isolated SB local-build test, use the resulting bin/BananaBrain.exe with
runtime native. Copy the imported AI text data into a fresh working directory's
bwapi-data/AI; create empty read and write directories. Preserve the original
Configuration.txt (tournament=true); drawing defaults to disabled. Start Protoss
first. Test only controlled names in an isolated profile.

This prototype has no storage hardening patches. Original read-before-write
precedence, parser bounds and opponent filename handling need review before
catalog admission. Fresh test profiles keep read empty so current write data can
be loaded on a later match. Existing BotSagi binaries and profiles are untouched.
