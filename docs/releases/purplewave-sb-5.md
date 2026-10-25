# PurpleWave sb.5 source update candidate

Status: review-only candidate. Source/distribution review and game validation are pending.
The published sb.4 catalog entry remains unchanged.

PurpleWave source advances from `a57d2511cc4f6318c2a2d61a504e8f58a7b090af`
(2026-08-22) to `88c1056aac722fd648ca81e5746da25ed8123c3e` (2026-10-15),
173 upstream commits. This is the upstream master revision checked on 2026-10-25.
The upstream MIT license is unchanged. JBWAPI, JBWEB, JavaJPS, mjson,
Scala, JNA, Java 21 requirements and the fixed configuration retain their pins.
The upstream mjson submodule update is not adopted: the recipe builds the separately
pinned mjson source from source-lock.json.

The runtime-isolation patch is rebased onto the updated source. Configuration
still comes from the assigned working profile, opponent names/history stay encoded,
and external visualizer launch stays disabled. The upstream opponent-specific
fixed-build selection and optional debug settings remain available; simviz remains
rejected by the existing fixed configuration loader.

Rebuild and package using the existing recipe with a fresh build directory:

```powershell
node tools/build-purplewave.ts purplewave-sb-5 <jdk-21-home>
node tools/package-purplewave.ts .build/purplewave-sb-5 purplewave-sb-5 --review
```

The current build recipes require committed inputs. Record the resulting recipe
revision, source/patch hashes, artifact hashes and actual validation scope here
before proposing admission. A familiar bot name and prior sb.4 test results do not
establish compatibility or approval for this candidate.
