# Android Portability Patches — OrcaSlicer `android/v2.3.1`

This branch = upstream tag `v2.3.1` + the patches below.

**Rule:** Patch only portability — never slicing behavior. Every behavioral change belongs upstream.
Upstream PRs are tracked in the last column; accepted PRs shrink this table permanently.

| # | Patch / file changed | What it does | Why upstream lacks it | Upstream PR |
|---|---|---|---|---|
| 1 | `deps/deps-android.cmake` (new) | Android NDK arm64-v8a cross-compilation platform for the dep build system | Upstream builds only target Linux/macOS/Win; Android is a new platform | — |
| 2 | `deps/CMakeLists.txt` | Android dispatch + skip GUI-only deps (wxWidgets, GLEW, GLFW, OpenCSG) | Deps that need a display cannot build on Android | — |
| 3 | `deps/OpenSSL/OpenSSL.cmake` | Android `android-arm64` Configure target | Non-cmake build needs explicit Android target, not auto-detect | — |
| 4 | `deps/CURL/CURL.cmake` | Android platform SSL flags | `CMAKE_SYSTEM_NAME=Android` not matched by existing Linux path | — |
| 5 | `deps/Boost/Boost.cmake` | Android arm64 aapcs context ABI | Boost.Context needs explicit ABI on arm64 Android | — |
| 6 | `CMakeLists.txt` | `SLIC3R_ANDROID` option: auto-sets `SLIC3R_GUI=OFF`, `SLIC3R_STATIC=ON`, `SLIC3R_PCH=OFF` | Engine has no Android-specific build mode | — |
| 7 | `src/CMakeLists.txt` + `src/libslic3r_android_init.cpp` | `liborcaslicer-core` shared library target for Android | Needed for JNI loading on Android | — |

## How to update to a new upstream tag

```bash
git fetch upstream --tags
git checkout -b android/vX.Y.Z vX.Y.Z
git cherry-pick <patch-commits>   # re-apply the series above onto the new tag
# Resolve conflicts, update this table, push
```

## References

- [PodSlicer app repo](https://github.com/alb-garj/podslicer) — consumes this branch as a submodule
- [OrcaSlicer upstream](https://github.com/OrcaSlicer/OrcaSlicer)
- [u1-slicer-for-android — NDK build recipes to crib from](https://github.com/taylormadearmy/u1-slicer-for-android)
