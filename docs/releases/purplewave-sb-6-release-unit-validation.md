# PurpleWave release-unit validation follow-up

The initial release-unit run failed because tests referenced APIs compiled only
with `debug_assertions`. [Test-only fix](https://github.com/ShieldBattery/ShieldBattery/pull/1842) matches those test/import
gates to their APIs and adds a release regression that rejects all eight debug
commands plus malformed payloads. Production code and release debug gates are
unchanged. CI now runs release tests and full Clippy on both architecture jobs.

| Profile | x64 tests | x86 tests | All-target/workspace Clippy |
| --- | ---: | ---: | --- |
| Debug | 640 | 640 | Passed on both |
| Release | 614 | 614 | Passed on both |

Both test matrices completed with zero failures or ignored tests. All eight
positive debug dispatch tests still execute on each debug target; the new
rejection test executes on each release target. Formatting and workflow YAML
validation passed. The isolated fix branch also passed 567 release x64 tests;
the full table above covers the combined PurpleWave integration.

Fix commit: `2b6994dfde5f3f5e75ce50b1918c64f42ff7067c`. Combined integration commit: `07595bd56318a5ad4272004c4e3cffac2bd13722`.
Both modified Rust files match upstream byte for byte before their test modules.
No gameplay behavior or bot package changed, so prior release DLL gameplay
results/hashes remain historical evidence rather than another claimed match run.
The [original failure](purplewave-sb-6-production-validation.json) is preserved
under `checks.extraReleaseUnitTests.initialAttempt`, alongside this successful
follow-up. Release-unit compilation is no longer an open check failure.
