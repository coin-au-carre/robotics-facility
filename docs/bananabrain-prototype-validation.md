# BananaBrain prototype validation

Tested on 2026-10-25. Local prototype only; no catalog entry or publication.

## Build

- Original AIIDE 2025 archive verified against `3d68f16ca785665c56ef78148271c5a1e19d1df82cdd1cf4620f3090ca207549`.
- Unmodified source/data import: `f66d7336ca61a0a0a1a984fad55c162d942694e4` on local branch
  `source/bananabrain-aiide2025`. That branch must be made available with any
  future contribution so its source pin can be fetched externally.
- Recipe: `393f86bbe5f9d8eebf873577f894d3548bcf1b67` on `prototype/bananabrain`.
- Existing native host and BWAPI client unchanged; no custom companion or bot
  source patch. All 37 original C++ compilation units built with MSVC as Win32
  Release/static CRT using the common build/provenance tooling.
- Executable SHA-256: `ff23054c8c9d9777aea9d131a689dc2b939982cd00c6d497223f1fe104750cca`.
- Metadata, typecheck, lint/format, catalog validation and all 66 tooling tests passed.

## Local SB tests

Used existing SB local-build/practice flow, a Win32 BananaBrain client and x64
StarCraft on Fighting Spirit 1.4. SB commit: `ff167db43053cead7a991a68c224c8d6bf61066d`.
Original AI text data and configuration were copied to an isolated working profile;
read started empty. Bot race was restricted to Protoss for this prototype.

| Run | Observations | End scope |
| --- | --- | --- |
| Opening | 16 Probes, two Pylons, Gateway, Assimilator and Cybernetics Core present; 1,233 accepted commands, zero rejected at frame 3,600 | Automatically stopped after frame 3,629; one result row saved |
| Same-profile restart | 7 Probes; 32 accepted commands, zero rejected at frame 720 | Automatically stopped after frame 734; two result rows saved |

The result-file hash before the second match matched the first match's saved
file. Original result-store code reloads that file and rewrites its result list;
growth from one to two rows confirms reuse in this controlled setup. Owned app
and game processes exited. These were deliberately stopped opening tests, not
naturally completed matches or strength measurements.

## Remaining before admission

Review opponent filenames, parser/file bounds, atomic writes, fixed configuration
and read/write precedence with pre-populated read data. Add review packaging,
notices and source-delivery checks before distribution. Test full matches,
learning reset and additional race/game-architecture combinations. The source
supports Terran and Zerg but neither was tested here.

Existing BotSagi binaries, profiles and the published catalog are unchanged.
No source-import or prototype branch was pushed.
