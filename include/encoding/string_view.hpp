// SPDX-License-Identifier: MIT
// File: encoding/string_view.hpp
// Author: 璀境石 (GitHub: @Demonese / @chuikingshek)
#pragma once

#ifndef PAINFUL_CPP_STRING_CONVERSION_ENCODING_STRING_VIEW_H
#define PAINFUL_CPP_STRING_CONVERSION_ENCODING_STRING_VIEW_H

#ifndef PAINFUL_CPP_STRING_CONVERSION_NAMESPACE
#define PAINFUL_CPP_STRING_CONVERSION_NAMESPACE ext
#endif

#include <string_view>

namespace PAINFUL_CPP_STRING_CONVERSION_NAMESPACE {
    // Does the compiler parse the source file with the correct encoding (utf-8) ?
    // If you are using MSVC compiler, please remember to add /utf-8 compile option.
    static_assert(sizeof("Ω") == 3 && ("Ω")[0] == '\xCE' && ("Ω")[1] == '\xA9');
    static_assert(sizeof("永") == 4 && ("永")[0] == '\xE6' && ("永")[1] == '\xB0' && ("永")[2] == '\xB8');
    // The C++ compiler you are using should support UTF-8 4-bytes
    static_assert(sizeof("🙂") == 5 && ("🙂")[0] == '\xF0' && ("🙂")[1] == '\x9F' && ("🙂")[2] == '\x99' && ("🙂")[3] == '\x82');

    // We require char to always be 8-bits
    static_assert(CHAR_BIT == 8);

    // We consider char and char8_t as the same type
    static_assert(sizeof(char) == sizeof(char8_t));

    // std::string_view

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string_view to_string_view(std::u8string_view const& s) {
        return { reinterpret_cast<std::string_view::const_pointer>(s.data()), s.size() };
    }

    // std::u8string_view

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::u8string_view to_u8string_view(std::string_view const& s) {
        return { reinterpret_cast<std::u8string_view::const_pointer>(s.data()), s.size() };
    }
}

#endif
