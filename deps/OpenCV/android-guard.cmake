# Prepends an add_android_project guard to OpenCV's samples/android/CMakeLists.txt.
# Called from OpenCV.cmake PATCH_COMMAND with:
#   -DANDROID=<bool> -DTARGET_FILE=<source_dir>/samples/android/CMakeLists.txt
# Safe to run on non-Android builds (no-op).
if(ANDROID AND EXISTS "${TARGET_FILE}")
    file(READ "${TARGET_FILE}" _content)
    if(NOT _content MATCHES "add_android_project")
        string(PREPEND _content "# add_android_project is provided by Android SDK Gradle plugin only.\n# When cross-compiling with NDK toolchain only, skip these samples.\nif(NOT COMMAND add_android_project)\n  return()\nendif()\n\n")
        file(WRITE "${TARGET_FILE}" "${_content}")
        message(STATUS "Applied Android guard to ${TARGET_FILE}")
    endif()
endif()
