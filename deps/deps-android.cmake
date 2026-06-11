# deps-android.cmake — Android NDK arm64-v8a cross-compilation platform
#
# Called automatically when CMAKE_SYSTEM_NAME=Android (set by NDK toolchain file).
# Invoked via deps-android/build-deps.sh which passes -DCMAKE_TOOLCHAIN_FILE
# pointing to $NDK/build/cmake/android.toolchain.cmake.
#
# OpenSSL's non-cmake configure script reads OPENSSL_ARCH to select the target.
# PIC is the Android default, so DEP_CMAKE_OPTS is empty.
# DEP_ANDROID_SKIP_GUI suppresses GUI-only deps (wxWidgets, GLEW, GLFW, OpenCSG).

set(OPENSSL_ARCH "android-arm64")
set(DEP_CMAKE_OPTS "")
set(DEP_ANDROID_SKIP_GUI ON)

include("deps-unix-common.cmake")
