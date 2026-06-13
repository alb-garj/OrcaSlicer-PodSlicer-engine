// android_gui_stubs.cpp — implementations normally provided by src/slic3r/ (GUI layer)
// that libslic3r core calls but that cannot be compiled with SLIC3R_GUI=OFF.
//
// Patch 32: two families of missing symbols at link time:
//   1. RGB2HSV  — pure math, defined in src/slic3r/Utils/ColorSpaceConvert.cpp
//      which includes <wx/colordlg.h> — unavailable in the NDK.
//   2. nsvg*    — nanosvg header-only library.  The desktop build defines
//      NANOSVG_IMPLEMENTATION in src/slic3r/GUI/BitmapCache.cpp; that file is
//      not compiled when SLIC3R_GUI=OFF.

#ifdef __ANDROID__

#include <cmath>
#include <algorithm>

// RGB2HSV: copied verbatim from src/slic3r/Utils/ColorSpaceConvert.cpp.
// FlushVolCalc.cpp (in libslic3r core) calls this function.
void RGB2HSV(float r, float g, float b, float* h, float* s, float* v)
{
    float Cmax  = std::max(std::max(r, g), b);
    float Cmin  = std::min(std::min(r, g), b);
    float delta = Cmax - Cmin;

    if (std::abs(delta) < 0.001f) {
        *h = 0.f;
    } else if (Cmax == r) {
        *h = 60.f * fmod((g - b) / delta, 6.f);
    } else if (Cmax == g) {
        *h = 60.f * ((b - r) / delta + 2.f);
    } else {
        *h = 60.f * ((r - g) / delta + 4.f);
    }

    *s = (std::abs(Cmax) < 0.001f) ? 0.f : delta / Cmax;
    *v = Cmax;
}

// nanosvg: provide the implementation that BitmapCache.cpp normally supplies.
// NSVGUtils.cpp (in libslic3r core) calls nsvgParseFromFile / nsvgParse / nsvgDelete.
#define NANOSVG_IMPLEMENTATION
#include "nanosvg/nanosvg.h"

#endif // __ANDROID__
