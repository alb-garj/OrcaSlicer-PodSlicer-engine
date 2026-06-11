// libslic3r_android_init.cpp
// Minimal Android entry point for liborcaslicer-core.so.
// JNI functions (SAPIL bridge) are added in P1.1 via engine-jni/.
// This translation unit ensures the shared library has at least one symbol.

#include "libslic3r/libslic3r.h"

extern "C" {

const char* orcaslicer_version() {
    return SLIC3R_VERSION;
}

} // extern "C"
