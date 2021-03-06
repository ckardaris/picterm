// SPDX-License-Identifier: Apache-2.0
// Copyright (C) 2020 Artem Senichev <artemsen@gmail.com>

#include "viewer.hpp"
#include "image_ldr.hpp"

#include <cstdio>
#include <cstdlib>
#include <exception>
#include <getopt.h>

/**
 * @brief Print help usage info.
 *
 * @param[in] app application's file name
 */
static void print_help(const char* app)
{
    puts("Picterm - preview images in terminal window.");
    printf("Usage: %s [OPTION...] FILE\n", app);
    puts("Default values are specified in brackets.");
    puts("  -b, --border=N         Window border size in pixels [0]");
    puts("  -s, --scale=PERCENT    Set initiial image scale [0:auto]");
    puts("  -e, --exit-unfocus     Exit if window lost focus [off]");
    puts("  -v, --version          Print version info and supported formats list");
    puts("  -h, --help             Print this help and exit");
}

/**
 * @brief Print version info.
 */
static void print_version()
{
    puts("Picterm ver." VERSION ".");
    puts("Project page: https://github.com/artemsen/picterm");
    puts("Image format support:");
    print_formats();
}

/** @brief Application entry point. */
int main(int argc, char* argv[])
{
    viewer view;

    // clang-format off
    const struct option longOpts[] = {
        {"border",       required_argument, nullptr, 'b'},
        {"scale",        required_argument, nullptr, 's'},
        {"exit-unfocus", no_argument,       nullptr, 'e'},
        {"version",      no_argument,       nullptr, 'v'},
        {"help",         no_argument,       nullptr, 'h'},
        {nullptr,        0,                 nullptr,  0 }
    };
    // clang-format on
    const char* shortOpts = "b:s:evh";

    opterr = 0; // prevent native error messages

    // parse arguments
    int val;
    while ((val = getopt_long(argc, argv, shortOpts, longOpts, nullptr)) != -1) {
        switch (val) {
            case 'b':
                view.border = atoi(optarg);
                break;
            case 's':
                view.scale = atoi(optarg);
                break;
            case 'e':
                view.exit_unfocus = true;
                break;
            case 'v':
                print_version();
                return EXIT_SUCCESS;
            case 'h':
                print_help(argv[0]);
                return EXIT_SUCCESS;
            default:
                fprintf(stderr, "Invalid option: %s\n", argv[optind - 1]);
                return EXIT_FAILURE;
            }
    }

    if (optind == argc) {
        fprintf(stderr, "File name expected, use `%s --help`.\n", argv[0]);
        return EXIT_FAILURE;
    }

    view.file_name = argv[optind];
    if (!*view.file_name) {
        fprintf(stderr, "File name can not be empty\n");
        return EXIT_FAILURE;
    }

    try {
        view.show();
    } catch (std::exception& ex) {
        fprintf(stderr, "Unable to preview file %s: %s\n", view.file_name, ex.what());
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
