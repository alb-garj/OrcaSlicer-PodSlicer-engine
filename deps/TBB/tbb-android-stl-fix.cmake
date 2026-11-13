# Patches TBB's hardcoded NDK ≤r25 libc++_shared.so path to work with NDK r28+
# (NDK r28 moved the file from sources/cxx-stl/llvm-libc++/libs/<ABI>/
#  to toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/)
# Run as: cmake -P tbb-android-stl-fix.cmake <SOURCE_DIR>/CMakeLists.txt
set(_file "${CMAKE_ARGV3}")
if(NOT EXISTS "${_file}")
    message(WARNING "TBB CMakeLists.txt not found at ${_file} — skipping NDK r28 STL path fix")
    return()
endif()
file(READ "${_file}" _content)
if(_content MATCHES "sources/cxx-stl/llvm-libc")
    string(REGEX REPLACE
        "sources/cxx-stl/llvm-libc\\+\\+/libs/[^/\"]+/libc\\+\\+_shared\\.so"
        "toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/lib/aarch64-linux-android/libc++_shared.so"
        _patched "${_content}")
    file(WRITE "${_file}" "${_patched}")
    message(STATUS "Applied NDK r28 libc++_shared.so path fix to TBB CMakeLists.txt")
endif()
