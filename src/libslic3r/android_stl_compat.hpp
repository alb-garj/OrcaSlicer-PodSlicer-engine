// android_stl_compat.hpp — force-included for all libslic3r TUs on Android.
//
// NDK r28's libc++ reorganized internal headers so that many std headers that
// were previously pulled in transitively (via <iostream>, <iomanip>, etc.) now
// require an explicit include.  Rather than patch each of the ~40 source files
// individually, we force-include this shim.
#pragma once
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <numeric>
#include <iterator>
#include <memory>
#include <stdexcept>
