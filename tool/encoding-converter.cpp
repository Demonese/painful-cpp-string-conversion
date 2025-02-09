// SPDX-License-Identifier: MIT
// Author: 璀境石 (GitHub: @Demonese / @chuikingshek)
#include <iostream>
#include <string>
#include <filesystem>
#include <span>
#include "encoding/string.hpp"
#ifdef _WIN32
#include <windows.h>
#endif

using std::string_literals::operator ""s;
using std::string_view_literals::operator ""sv;

namespace {
    [[nodiscard]] bool is_help_option(std::string_view const& option) {
        return option == "-h"sv || option == "--help"sv;
    }
    void help() {
        std::cout
            << "Usage: encoding-converter [options]\n"sv
            << "Options:\n"sv
            << "\t-h --help\t\tShow this help text\n"sv
            << "\t-v --version\t\tShow version information\n"sv
            << "\t-i --input\t\tInput file\n"sv
            << "\t-o --output\t\tOutput file (If not specified, default to input file)\n"sv
            << "\t-e --encoding\t\tText encoding of output file (If not specified, default to \"utf8\")\n"sv
            << "Encoding:\n"sv
            << "\tutf8\t\tUTF-8 without byte order mark\n"sv
            << "\tutf8bom\t\tUTF-8 with byte order mark\n"sv
            << "\tutf16le\t\tUTF-16 little endian (with byte order mark)\n"sv
            << "\tutf16be\t\tUTF-16 big endian (with byte order mark)\n"sv
            << "\tutf32le\t\tUTF-32 little endian (with byte order mark)\n"sv
            << "\tutf32be\t\tUTF-32 big endian (with byte order mark)\n"sv
            << ""sv;
    }
    [[nodiscard]] bool is_version_option(std::string_view const& option) {
        return option == "-v"sv || option == "--version"sv;
    }
    void version() {
        std::cout << "encoding-converter version 1.0.0\n"sv;
    }
    [[nodiscard]] bool is_input_option(std::string_view const& option) {
        return option == "-i"sv || option == "--input"sv;
    }
    [[nodiscard]] bool is_output_option(std::string_view const& option) {
        return option == "-o"sv || option == "--output"sv;
    }
    [[nodiscard]] bool is_encoding_option(std::string_view const& option) {
        return option == "-e"sv || option == "--encoding"sv;
    }
    enum class encoding_type {
        unknown,
        utf8,
        utf8bom,
        utf16le,
        utf16be,
        utf32le,
        utf32be,
    };
    [[nodiscard]] encoding_type to_encoding_type(std::string_view const& value) {
        if (value == "utf8"sv) {
            return encoding_type::utf8;
        }
        if (value == "utf8bom"sv) {
            return encoding_type::utf8bom;
        }
        if (value == "utf16le"sv) {
            return encoding_type::utf16le;
        }
        if (value == "utf16be"sv) {
            return encoding_type::utf16be;
        }
        if (value == "utf32le"sv) {
            return encoding_type::utf32le;
        }
        if (value == "utf32be"sv) {
            return encoding_type::utf32be;
        }
        return encoding_type::unknown;
    }
    [[nodiscard]] std::string_view to_string_view(encoding_type const enc_type) {
        switch (enc_type) {
        case encoding_type::utf8: return "utf8"sv;
        case encoding_type::utf8bom: return "utf8bom"sv;
        case encoding_type::utf16le: return "utf16le"sv;
        case encoding_type::utf16be: return "utf16be"sv;
        case encoding_type::utf32le: return "utf32le"sv;
        case encoding_type::utf32be: return "utf32be"sv;
        default: return "unknown"sv;
        }
    }
}

int main(int const argc, char const* const* const argv) {
    std::filesystem::path input;
    std::filesystem::path output;
    encoding_type output_encoding{encoding_type::utf8};
    std::span const args(argv, static_cast<size_t>(argc));
    for (size_t i = 0; i < args.size(); i += 1) {
        std::string_view const arg(args[i]);
        if (is_help_option(arg)) {
            help();
            return 0;
        }
        if (is_version_option(arg)) {
            version();
            return 0;
        }
        if (is_input_option(arg)) {
            if (i + 1 < args.size()) {
                std::string_view const value(args[i + 1]);
                input.assign(value);
            }
        }
        if (is_output_option(arg)) {
            if (i + 1 < args.size()) {
                std::string_view const value(args[i + 1]);
                output.assign(value);
            }
        }
        if (is_encoding_option(arg)) {
            if (i + 1 < args.size()) {
                std::string_view const value(args[i + 1]);
                output_encoding = to_encoding_type(value);
                if (output_encoding == encoding_type::unknown) {
                    std::cout << "Unsupported encoding: " << value << std::endl;
                    return 1;
                }
            }
        }
    }
    if (input.empty()) {
        std::cout << "No input file specified" << std::endl;
        return 1;
    }
    if (std::error_code ec; !std::filesystem::is_regular_file(input, ec)) {
        std::cout << "File not exist: " << input << std::endl;
        return 1;
    }
    if (output.empty()) {
        output.assign(input);
    }
#ifndef NDEBUG
    std::cout << "Details:" << std::endl;
    std::cout << "\tInput file: " << input << std::endl;
    std::cout << "\tOutput file: " << output << std::endl;
    std::cout << "\tOutput file encoding: " << to_string_view(output_encoding) << std::endl;
#endif
    return 0;
}
