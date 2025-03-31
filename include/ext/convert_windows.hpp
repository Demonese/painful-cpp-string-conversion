// SPDX-License-Identifier: MIT
// Project: Painful C++ string conversion
// Url: https://github.com/Demonese/painful-cpp-string-conversion
// Author: 璀境石 (https://github.com/Demonese)
// File: ext/convert_windows.hpp
#pragma once

#ifndef PAINFUL_CPP_STRING_CONVERSION_EXT_CONVERT_WINDOWS_H
#define PAINFUL_CPP_STRING_CONVERSION_EXT_CONVERT_WINDOWS_H

#include "ext/convert.hpp"

#ifndef PAINFUL_CPP_STRING_CONVERSION_NAMESPACE
#define PAINFUL_CPP_STRING_CONVERSION_NAMESPACE ext
#endif

#ifndef PAINFUL_CPP_STRING_CONVERSION_FUNCTION
#define PAINFUL_CPP_STRING_CONVERSION_FUNCTION convert
#endif

namespace PAINFUL_CPP_STRING_CONVERSION_NAMESPACE {
    // We consider wchar_t and char16_t as the same type
    static_assert(sizeof(wchar_t) == sizeof(char16_t));

    template<> inline std::wstring_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string_view const& s) {
        return { reinterpret_cast<std::wstring_view::const_pointer>(s.data()), s.size() };
    }

    template<> inline std::wstring_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string const& s) {
        return { reinterpret_cast<std::wstring_view::const_pointer>(s.data()), s.size() };
    }

    template<> inline std::u16string_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring_view const& s) {
        return { reinterpret_cast<std::u16string_view::const_pointer>(s.data()), s.size() };
    }

    template<> inline std::u16string_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring const& s) {
        return { reinterpret_cast<std::u16string_view::const_pointer>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring_view const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::string>(PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string_view>(s));
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::string>(PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string_view>(s));
    }

    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring_view const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u8string>(PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string_view>(s));
    }

    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u8string>(PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string_view>(s));
    }

    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring_view const& s) {
        return { reinterpret_cast<std::u16string::const_pointer>(s.data()), s.size() };
    }

    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring const& s) {
        return { reinterpret_cast<std::u16string::const_pointer>(s.data()), s.size() };
    }

    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring_view const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u32string>(PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string_view>(s));
    }

    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::wstring const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u32string>(PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string_view>(s));
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string_view const& s) {
        std::wstring buffer;
        auto p = reinterpret_cast<char8_t const*>(s.data());
        size_t n = s.size();
        size_t o{};
        char32_t c{};
        char16_t t[4]{};
        size_t m{};
        while (n > 0) {
            o = details::utf8_to_utf32(p, n, c);
            p += o;
            n -= o;
            m = details::utf32_to_utf16(c, t);
            buffer.append(reinterpret_cast<std::wstring::const_pointer>(t), m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::wstring>(std::string_view(s));
    }

    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string_view const& s) {
        std::wstring buffer;
        char8_t const* p = s.data();
        size_t n = s.size();
        size_t o{};
        char32_t c{};
        char16_t t[4]{};
        size_t m{};
        while (n > 0) {
            o = details::utf8_to_utf32(p, n, c);
            p += o;
            n -= o;
            m = details::utf32_to_utf16(c, t);
            buffer.append(reinterpret_cast<std::wstring::const_pointer>(t), m);
        }
        return buffer;
    }

    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::wstring>(std::u8string_view(s));
    }

    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string_view const& s) {
        return { reinterpret_cast<std::wstring::const_pointer>(s.data()), s.size() };
    }

    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string const& s) {
        return { reinterpret_cast<std::wstring::const_pointer>(s.data()), s.size() };
    }

    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string_view const& s) {
        std::wstring buffer;
        char16_t t[4]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf16(c, t);
            buffer.append(reinterpret_cast<std::wstring::const_pointer>(t), m);
        }
        return buffer;
    }

    template<> inline std::wstring PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::wstring>(std::u32string_view(s));
    }
}

#endif
