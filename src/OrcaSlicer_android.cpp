// OrcaSlicer_android.cpp — Android CLI harness for on-device slice validation.
//
// Patch 30: replaces the desktop OrcaSlicer.cpp as the orca-cli source on Android.
// The desktop CLI pulls in wxWidgets, PartPlate, OpenGL and GLFW, none of which
// are available in the NDK cross-build.  This file calls libslic3r directly:
//   load_3mf → Print::apply → Print::process → Print::export_gcode
//
// Usage:
//   orca-cli --slice <input.3mf> [--output <out.gcode>] [--threads <n>]
//   orca-cli --version
//
// --threads 1 caps the TBB global thread pool to a single worker (via
// Slic3r::disable_multi_threading()) so the slice is fully deterministic —
// no TBB task-stealing order to introduce run-to-run or cross-arch noise.
// Used by the host Linux baseline for arm64-vs-Linux byte-diff parity checks.

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

#include <boost/log/trivial.hpp>
#include <boost/filesystem.hpp>
#include <boost/nowide/iostream.hpp>

#include "libslic3r/libslic3r.h"
#include "libslic3r/Config.hpp"
#include "libslic3r/Model.hpp"
#include "libslic3r/Print.hpp"
#include "libslic3r/Format/3mf.hpp"
#include "libslic3r/PrintConfig.hpp"
#include "libslic3r/Utils.hpp"

static void usage(const char* prog) {
    boost::nowide::cout
        << "Usage:\n"
        << "  " << prog << " --slice <input.3mf> [--output <out.gcode>] [--threads <n>]\n"
        << "  " << prog << " --version\n";
}

int main(int argc, char** argv) {
    const char* input_path  = nullptr;
    const char* output_path = nullptr;
    bool        do_slice    = false;
    int         threads     = 0; // 0 = engine default concurrency

    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "--version") == 0) {
            boost::nowide::cout << "orca-cli " SLIC3R_VERSION " (Android arm64)\n";
            return 0;
        } else if (std::strcmp(argv[i], "--slice") == 0 && i + 1 < argc) {
            input_path = argv[++i];
            do_slice   = true;
        } else if (std::strcmp(argv[i], "--output") == 0 && i + 1 < argc) {
            output_path = argv[++i];
        } else if (std::strcmp(argv[i], "--threads") == 0 && i + 1 < argc) {
            threads = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "--help") == 0 || std::strcmp(argv[i], "-h") == 0) {
            usage(argv[0]);
            return 0;
        } else {
            boost::nowide::cerr << "Unknown argument: " << argv[i] << "\n";
            usage(argv[0]);
            return 1;
        }
    }

    if (!do_slice) {
        usage(argv[0]);
        return 1;
    }

    if (threads == 1) {
        // Only single-thread forcing is wired up (the determinism use case);
        // other thread counts would need tbb::global_control with that value.
        Slic3r::disable_multi_threading();
    } else if (threads > 1) {
        boost::nowide::cerr << "--threads values other than 1 are not supported; "
                                "ignoring and using engine default concurrency.\n";
    }

    if (!boost::filesystem::exists(input_path)) {
        boost::nowide::cerr << "Input file not found: " << input_path << "\n";
        return 2;
    }

    // Derive output path from input if not given.
    std::string out;
    if (output_path) {
        out = output_path;
    } else {
        out = boost::filesystem::path(input_path).replace_extension(".gcode").string();
    }

    BOOST_LOG_TRIVIAL(info) << "orca-cli: slicing " << input_path << " -> " << out;

    Slic3r::DynamicPrintConfig          config;
    Slic3r::ConfigSubstitutionContext   subst(Slic3r::ForwardCompatibilitySubstitutionRule::Enable);
    Slic3r::Model                       model;

    if (!Slic3r::load_3mf(input_path, config, subst, &model, false)) {
        boost::nowide::cerr << "Failed to load 3MF: " << input_path << "\n";
        return 3;
    }

    if (model.objects.empty()) {
        boost::nowide::cerr << "No objects found in 3MF.\n";
        return 4;
    }

    config.normalize_fdm();

    Slic3r::Print print;
    print.apply(model, config);

    try {
        print.process();
    } catch (const std::exception& ex) {
        boost::nowide::cerr << "Slicing failed: " << ex.what() << "\n";
        return 5;
    }

    std::string result_path = print.export_gcode(out, nullptr, nullptr);
    if (result_path.empty()) {
        boost::nowide::cerr << "G-code export failed.\n";
        return 6;
    }

    BOOST_LOG_TRIVIAL(info) << "orca-cli: done -> " << result_path;
    boost::nowide::cout << "OK " << result_path << "\n";
    return 0;
}
