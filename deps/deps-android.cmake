# deps-android.cmake — Android NDK arm64-v8a cross-compilation platform
#
# Called automatically when CMAKE_SYSTEM_NAME=Android (set by NDK toolchain file).
# Invoked via deps-android/build-deps.sh which passes -DCMAKE_TOOLCHAIN_FILE
# pointing to $NDK/build/cmake/android.toolchain.cmake.
#
# OpenSSL's non-cmake configure script reads OPENSSL_ARCH to select the target.
# GUI-only deps (wxWidgets, GLEW, GLFW, OpenCSG) are skipped via if(NOT ANDROID) guards in deps/CMakeLists.txt.

set(OPENSSL_ARCH "android-arm64")

# Cap build parallelism on Android/WSL2: large deps (OCCT) launch ~NPROC parallel clang++
# processes each loading the full OCCT header set (~400-700 MB/process).  With the host
# reporting 16 CPUs and WSL2 having ~8 GB RAM, -j16 exhausts memory and segfaults.
# -j4 uses ~2.8 GB and leaves comfortable headroom.  Other platforms are unaffected.
if(NPROC GREATER 4)
    set(NPROC 4)
endif()

# Forward ANDROID_ABI/PLATFORM to all sub-project cmake invocations.
# Without these, inner builds (Boost, OpenCV, etc.) default to armeabi-v7a
# because the NDK toolchain only gets CMAKE_TOOLCHAIN_FILE, not the ABI variables.
set(DEP_CMAKE_OPTS
    "-DANDROID_ABI=${ANDROID_ABI}"
    "-DANDROID_PLATFORM=${ANDROID_PLATFORM}"
    "-DANDROID_STL=c++_shared"
)

# Used by GMP.cmake / MPFR.cmake autoconf configure --host= arg
set(TOOLCHAIN_PREFIX "aarch64-linux-android")

# Versioned clang wrappers (e.g. aarch64-linux-android26-clang) have --target baked in.
# Bare CMAKE_C_COMPILER is generic clang — it compiles for the build host when
# called by autoconf configure without explicit --target, so GMP/MPFR configure
# C++ checks fail. Derive from the cmake compiler path and ANDROID_PLATFORM.
get_filename_component(_toolchain_bin "${CMAKE_C_COMPILER}" DIRECTORY)
string(REPLACE "android-" "" _android_api "${ANDROID_PLATFORM}")
set(ANDROID_AUTOCONF_CC  "${_toolchain_bin}/aarch64-linux-android${_android_api}-clang")
set(ANDROID_AUTOCONF_CXX "${_toolchain_bin}/aarch64-linux-android${_android_api}-clang++")

include("deps-unix-common.cmake")
