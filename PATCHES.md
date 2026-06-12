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
| 8 | `src/libslic3r/Platform.hpp` + `Platform.cpp` | `Platform::Android` enum + `#elif defined(__ANDROID__)` detection before Linux branch | Android defines `__linux__` but needs distinct identity for runtime decisions | — |
| 9 | `src/libslic3r/GCode/PostProcessor.cpp` | Guard `boost::process` with `#ifndef __ANDROID__`; Android stub returns 0 | `boost::process` requires `fork/exec` not available on Android/Bionic | — |
| 10 | `src/OrcaSlicer.cpp` | Android guard for `boost::dll::program_location()` → `argv[0]`; `boost::dll` is unreliable on Android | `boost::dll` ELF/dl_iterate_phdr internals not available on all Android versions | — |
| 11 | `src/CMakeLists.txt` | `orca-cli` native executable target; links `orcaslicer-core.so`; 16 KB aligned | Android CLI harness for adb-based on-device slice validation | — |
| 12 | `deps/deps-android.cmake` | Forward `ANDROID_ABI`/`ANDROID_PLATFORM`/`ANDROID_STL` to all sub-project cmake invocations via `DEP_CMAKE_OPTS`; add versioned clang wrapper vars (`ANDROID_AUTOCONF_CC/CXX`) for autoconf-based deps | Sub-projects only received `CMAKE_TOOLCHAIN_FILE` which defaults to armeabi-v7a; bare clang fails autoconf C++ checks | — |
| 13 | `deps/GMP/GMP.cmake` + `deps/MPFR/MPFR.cmake` | Use `ANDROID_AUTOCONF_CC/CXX` (versioned `aarch64-linux-android26-clang` wrappers) as CC/CXX for autoconf configure | Bare clang without `--target` builds for x86_64 host, failing GMP/MPFR configure C++ compiler tests | — |
| 14 | `deps/OpenCV/android-guard.cmake` (new) + `deps/OpenCV/OpenCV.cmake` | Replace git-apply 0003 patch with cmake `-P` script that prepends `add_android_project` guard at build time | `git apply --directory` with `../` relative path resolves outside the engine git repo; cmake FILE() is path-independent | — |
| 15 | `deps/TBB/tbb-android-stl-fix.cmake` (new) + `deps/TBB/TBB.cmake` | PATCH_COMMAND cmake script rewrites TBB's `configure_file` path from `sources/cxx-stl/llvm-libc++/libs/…` to `toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/` | NDK r28 moved `libc++_shared.so`; TBB 2021.5.0 hardcodes the old pre-r28 path | — |
| 16 | `deps/CURL/CURL.cmake` | Android section: pass explicit `-DOPENSSL_CRYPTO_LIBRARY/-DOPENSSL_SSL_LIBRARY/-DOPENSSL_INCLUDE_DIR=${DESTDIR}/…`; add `add_dependencies(dep_CURL dep_OpenSSL)` | NDK toolchain sets `CMAKE_FIND_ROOT_PATH_MODE_{INCLUDE,LIBRARY}=ONLY` → `CMAKE_PREFIX_PATH` is ignored; cross-compiled OpenSSL in destdir is never found without explicit paths | — |
| 17 | `deps/GMP/GMP.cmake` + `deps/MPFR/MPFR.cmake` | Pass `MAKEINFO=true` to `make` BUILD_COMMAND and INSTALL_COMMAND | GMP/MPFR build the `doc/` subdirectory which requires `makeinfo` (texinfo); when building on NTFS via WSL2, .texi timestamps appear modified and make tries to regenerate .info files — overriding `MAKEINFO` to the no-op `true` suppresses this without skipping the library build | — |
| 18 | `deps/MPFR/MPFR.cmake` | Skip `autoreconf -f -i` when `ANDROID`; add `--disable-maintainer-mode` to configure | MPFR 4.2.2 ships a pre-generated configure script so autoreconf is unneeded; NTFS timestamps on /mnt/c/ cause make to think `configure.ac`/`Makefile.am` are newer than generated files — enabling maintainer mode causes make to invoke autoconf/automake which are not installed | — |
| 19 | `deps/deps-android.cmake` | Cap `NPROC` to 4 when `NPROC > 4` for Android | OCCT cross-compilation with -j16 exhausts WSL2 RAM (8GB; ~400-700MB per clang++ process × 16 = OOM segfault); -j4 uses ~2.8 GB leaving comfortable headroom | — |
| 20 | `deps/CMakeLists.txt` | Guard `ProcessorCount(NPROC)` with `if(NOT DEFINED NPROC)`; add `elseif(ANDROID)` block inside `orcaslicer_add_cmake_project` that caps `_build_j` to `-j4` if the regex-extracted count exceeds 4 | `ProcessorCount()` calls `set()` which can shadow the `-DNPROC` cache variable inside a cmake function scope, silently restoring -j16. The in-function cap catches this regardless of scope resolution. Combined with `build-deps.sh` capping the shell NPROC and passing `-DNPROC=4` to cmake, ensures OCCT inner ninja invocation uses -j4 | — |

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
