// SPDX-License-Identifier: MIT
// Project: Painful C++ string conversion
// Url: https://github.com/Demonese/painful-cpp-string-conversion
// Author: 璀境石 (https://github.com/Demonese)
// File: ext/convert.hpp
#pragma once

#ifndef PAINFUL_CPP_STRING_CONVERSION_EXT_CONVERT_H
#define PAINFUL_CPP_STRING_CONVERSION_EXT_CONVERT_H

#include <string>
#include <string_view>

#ifndef PAINFUL_CPP_STRING_CONVERSION_NAMESPACE
#define PAINFUL_CPP_STRING_CONVERSION_NAMESPACE ext
#endif

#ifndef PAINFUL_CPP_STRING_CONVERSION_FUNCTION
#define PAINFUL_CPP_STRING_CONVERSION_FUNCTION convert
#endif

namespace PAINFUL_CPP_STRING_CONVERSION_NAMESPACE {
    // We require char to always be 8-bits
    static_assert(CHAR_BIT == 8);

    // We consider char and char8_t as the same type
    static_assert(sizeof(char) == sizeof(char8_t));

    namespace details {
        [[nodiscard]] constexpr uint8_t operator""_u8(unsigned long long int const v) noexcept { return static_cast<uint8_t>(v); }

        [[nodiscard]] inline bool is_utf8_1(char32_t const c) noexcept { return c <= 0x7fu; }
        [[nodiscard]] inline bool is_utf8_2(char32_t const c) noexcept { return c > 0x7fu && c <= 0x7ffu; }
        [[nodiscard]] inline bool is_utf8_3(char32_t const c) noexcept { return c > 0x7ffu && c <= 0xffffu; }
        [[nodiscard]] inline bool is_utf8_4(char32_t const c) noexcept { return c > 0xffffu && c <= 0x1f'ffffu; }
        [[nodiscard]] inline bool is_utf8_5(char32_t const c) noexcept { return c > 0x1f'ffffu && c <= 0x3ff'ffffu; }
        [[nodiscard]] inline bool is_utf8_6(char32_t const c) noexcept { return c > 0x3ff'ffffu && c <= 0x7fff'ffffu; }

        [[nodiscard]] inline char8_t e_utf8_1(char32_t const c) noexcept { return c & 0b0111'1111_u8; }
        [[nodiscard]] inline char8_t e_utf8_2(char32_t const c) noexcept { return 0b1100'0000_u8 | (((c >> (6 * 1)) & 0b0001'1111_u8)); }
        [[nodiscard]] inline char8_t e_utf8_3(char32_t const c) noexcept { return 0b1110'0000_u8 | (((c >> (6 * 2)) & 0b0000'1111_u8)); }
        [[nodiscard]] inline char8_t e_utf8_4(char32_t const c) noexcept { return 0b1111'0000_u8 | (((c >> (6 * 3)) & 0b0000'0111_u8)); }
        [[nodiscard]] inline char8_t e_utf8_5(char32_t const c) noexcept { return 0b1111'1000_u8 | (((c >> (6 * 4)) & 0b0000'0011_u8)); }
        [[nodiscard]] inline char8_t e_utf8_6(char32_t const c) noexcept { return 0b1111'1100_u8 | (((c >> (6 * 5)) & 0b0000'0001_u8)); }
        [[nodiscard]] inline char8_t e_utf8_m(char32_t const c, int const unit) noexcept { return 0b1000'0000_u8 | ((c >> (6 * unit)) & 0b0011'1111_u8); }

        [[nodiscard]] inline bool is_utf16_h(char32_t const c) noexcept { return c >= 0xd800u && c <= 0xdbffu; }
        [[nodiscard]] inline bool is_utf16_l(char32_t const c) noexcept { return c >= 0xdc00u && c <= 0xdfffu; }

        [[nodiscard]] inline size_t utf32_to_utf8(char32_t const c, char8_t (&s)[8], bool const extended = false) noexcept {
            if (is_utf8_1(c)) {
                s[0] = e_utf8_1(c);
                s[1] = {};
                return 1;
            }
            if (is_utf8_2(c)) {
                s[0] = e_utf8_2(c);
                s[1] = e_utf8_m(c, 0);
                s[2] = {};
                return 2;
            }
            if (!is_utf16_h(c) && !is_utf16_l(c) && is_utf8_3(c)) {
                s[0] = e_utf8_3(c);
                s[1] = e_utf8_m(c, 1);
                s[2] = e_utf8_m(c, 0);
                s[3] = {};
                return 3;
            }
            if (is_utf8_4(c)) {
                s[0] = e_utf8_4(c);
                s[1] = e_utf8_m(c, 2);
                s[2] = e_utf8_m(c, 1);
                s[3] = e_utf8_m(c, 0);
                s[4] = {};
                return 4;
            }
            if (extended && is_utf8_5(c)) {
                s[0] = e_utf8_5(c);
                s[1] = e_utf8_m(c, 3);
                s[2] = e_utf8_m(c, 2);
                s[3] = e_utf8_m(c, 1);
                s[4] = e_utf8_m(c, 0);
                s[5] = {};
                return 5;
            }
            if (extended && is_utf8_6(c)) {
                s[0] = e_utf8_6(c);
                s[1] = e_utf8_m(c, 4);
                s[2] = e_utf8_m(c, 3);
                s[3] = e_utf8_m(c, 2);
                s[4] = e_utf8_m(c, 1);
                s[5] = e_utf8_m(c, 0);
                s[6] = {};
                return 6;
            }
            s[0] = u8'?';
            s[1] = {};
            return 1;
        }

        [[nodiscard]] inline bool is_utf8_1(char8_t const c) noexcept { return (c & 0x80_u8) == 0x00_u8; }
        [[nodiscard]] inline bool is_utf8_2(char8_t const c) noexcept { return (c & 0xe0_u8) == 0xc0_u8; }
        [[nodiscard]] inline bool is_utf8_3(char8_t const c) noexcept { return (c & 0xf0_u8) == 0xe0_u8; }
        [[nodiscard]] inline bool is_utf8_4(char8_t const c) noexcept { return (c & 0xf8_u8) == 0xf0_u8; }
        [[nodiscard]] inline bool is_utf8_5(char8_t const c) noexcept { return (c & 0xfc_u8) == 0xf8_u8; }
        [[nodiscard]] inline bool is_utf8_6(char8_t const c) noexcept { return (c & 0xfe_u8) == 0xfc_u8; }
        [[nodiscard]] inline bool is_utf8_m(char8_t const c) noexcept { return (c & 0xc0_u8) == 0x80_u8; }

        [[nodiscard]] inline char32_t d_utf8_2(char8_t const c) noexcept { return (c & 0x1fu) << (6 * 1); }
        [[nodiscard]] inline char32_t d_utf8_3(char8_t const c) noexcept { return (c & 0x0fu) << (6 * 2); }
        [[nodiscard]] inline char32_t d_utf8_4(char8_t const c) noexcept { return (c & 0x07u) << (6 * 3); }
        [[nodiscard]] inline char32_t d_utf8_5(char8_t const c) noexcept { return (c & 0x03u) << (6 * 4); }
        [[nodiscard]] inline char32_t d_utf8_6(char8_t const c) noexcept { return (c & 0x01u) << (6 * 5); }
        [[nodiscard]] inline char32_t d_utf8_m(char8_t const c, int const unit) noexcept { return (c & 0x3fu) << (6 * unit); }

        [[nodiscard]] inline size_t utf8_to_utf32(char8_t const* const s, size_t const n, char32_t& c, bool const extended = false) noexcept {
            if (is_utf8_1(s[0])) {
                c = s[0];
                return 1;
            }
            if (n >= 2 && is_utf8_2(s[0])) {
                if (is_utf8_m(s[1])) {
                    c = d_utf8_2(s[0]) | d_utf8_m(s[1], 0);
                    return 2;
                }
            }
            else if (n >= 3 && is_utf8_3(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2])) {
                    c = d_utf8_3(s[0]) | d_utf8_m(s[1], 1) | d_utf8_m(s[2], 0);
                    return 3;
                }
            }
            else if (n >= 4 && is_utf8_4(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2]) && is_utf8_m(s[3])) {
                    c = d_utf8_4(s[0]) | d_utf8_m(s[1], 2) | d_utf8_m(s[2], 1) | d_utf8_m(s[3], 0);
                    return 4;
                }
            }
            else if (extended && n >= 5 && is_utf8_5(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2]) && is_utf8_m(s[3]) && is_utf8_m(s[4])) {
                    c = d_utf8_5(s[0]) | d_utf8_m(s[1], 3) | d_utf8_m(s[2], 2) | d_utf8_m(s[3], 1) | d_utf8_m(s[4], 0);
                    return 5;
                }
            }
            else if (extended && n >= 6 && is_utf8_6(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2]) && is_utf8_m(s[3]) && is_utf8_m(s[4]) && is_utf8_m(s[5])) {
                    c = d_utf8_6(s[0]) | d_utf8_m(s[1], 4) | d_utf8_m(s[2], 3) | d_utf8_m(s[3], 2) | d_utf8_m(s[4], 1) | d_utf8_m(s[5], 0);
                    return 6;
                }
            }
            c = U'?';
            return 1;
        }

        [[nodiscard]] inline bool is_utf16_h(char16_t const c) noexcept { return (c & 0xfc00u) == 0xd800u; }
        [[nodiscard]] inline bool is_utf16_l(char16_t const c) noexcept { return (c & 0xfc00u) == 0xdc00u; }

        [[nodiscard]] inline char32_t utf16_to_utf32(char16_t const h, char16_t const l) noexcept {
            return 0x10000u + ((h & 0x3ffu) << 10) | (l & 0x3ffu);
        }

        [[nodiscard]] inline size_t utf16_to_utf32(char16_t const* const s, size_t const n, char32_t& c) {
            if (n >= 2 && is_utf16_h(s[0]) && is_utf16_l(s[1])) {
                c = utf16_to_utf32(s[0], s[1]);
                return 2;
            }
            if (!is_utf16_h(s[0]) && !is_utf16_l(s[0])) {
                c = s[0];
                return 1;
            }
            c = U'?';
            return 1;
        }

        [[nodiscard]] inline size_t utf32_to_utf16(char32_t const c, char16_t (&s)[4]) noexcept {
            if (!is_utf16_h(c) && !is_utf16_l(c) && c <= 0xffffu) {
                s[0] = c & 0xffffu;
                s[1] = {};
                return 1;
            }
            if (c > 0xffffu && c <= 0x10'ffffu) {
                char32_t const d = c - 0x1'0000u;
                s[0] = 0xd800u | ((d >> 10) & 0x3ffu);
                s[1] = 0xdc00u | (d & 0x3ffu);
                s[2] = {};
                return 2;
            }
            s[0] = u'?';
            s[1] = 0;
            return 1;
        }
    }

    // convert template

    template<typename Target, typename Source>
    Target PAINFUL_CPP_STRING_CONVERSION_FUNCTION(Source const& s);

    // std::string_view <---> std::u8string_view

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string_view const& s) {
        return { reinterpret_cast<std::string_view::const_pointer>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string const& s) {
        return { reinterpret_cast<std::string_view::const_pointer>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u8string_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string_view const& s) {
        return { reinterpret_cast<std::u8string_view::const_pointer>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u8string_view PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string const& s) {
        return { reinterpret_cast<std::u8string_view::const_pointer>(s.data()), s.size() };
    }

    // std::string

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string_view const& s) {
        return { reinterpret_cast<std::string::const_pointer>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string const& s) {
        return { reinterpret_cast<std::string::const_pointer>(s.c_str()), s.length() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string_view const& s) {
        std::string buffer;
        char16_t const* p = s.data();
        size_t n = s.size();
        size_t o{};
        char32_t c{};
        char8_t t[8]{};
        size_t m{};
        while (n > 0) {
            o = details::utf16_to_utf32(p, n, c);
            p += o;
            n -= o;
            m = details::utf32_to_utf8(c, t);
            buffer.append(reinterpret_cast<std::string::const_pointer>(t), m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::string>(std::u16string_view(s));
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string_view const& s) {
        std::string buffer;
        char8_t t[8]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf8(c, t);
            buffer.append(reinterpret_cast<std::string::const_pointer>(t), m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::string>(std::u32string_view(s));
    }

    // std::u8string

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string_view const& s) {
        return { reinterpret_cast<std::u8string::const_pointer>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string const& s) {
        return { reinterpret_cast<std::u8string::const_pointer>(s.c_str()), s.length() };
    }

    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string_view const& s) {
        std::u8string buffer;
        char16_t const* p = s.data();
        size_t n = s.size();
        size_t o{};
        char32_t c{};
        char8_t t[8]{};
        size_t m{};
        while (n > 0) {
            o = details::utf16_to_utf32(p, n, c);
            p += o;
            n -= o;
            m = details::utf32_to_utf8(c, t);
            buffer.append(t, m);
        }
        return buffer;
    }

    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u8string>(std::u16string_view(s));
    }

    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string_view const& s) {
        std::u8string buffer;
        char8_t t[8]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf8(c, t);
            buffer.append(t, m);
        }
        return buffer;
    }

    template<> inline std::u8string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u8string>(std::u32string_view(s));
    }

    // std::u16string

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string_view const& s) {
        std::u16string buffer;
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
            buffer.append(t, m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string>(std::string_view(s));
    }

    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string_view const& s) {
        std::u16string buffer;
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
            buffer.append(t, m);
        }
        return buffer;
    }

    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string>(std::u8string_view(s));
    }

    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string_view const& s) {
        std::u16string buffer;
        char16_t t[4]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf16(c, t);
            buffer.append(t, m);
        }
        return buffer;
    }

    template<> inline std::u16string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u32string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u16string>(std::u32string_view(s));
    }

    // std::u32string

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string_view const& s) {
        std::u32string buffer;
        auto p = reinterpret_cast<char8_t const*>(s.data());
        size_t n = s.size();
        size_t o{};
        char32_t c{};
        while (n > 0) {
            o = details::utf8_to_utf32(p, n, c);
            p += o;
            n -= o;
            buffer.push_back(c);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u32string>(std::string_view(s));
    }

    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string_view const& s) {
        std::u32string buffer;
        char8_t const* p = s.data();
        size_t n = s.size();
        size_t o{};
        char32_t c{};
        while (n > 0) {
            o = details::utf8_to_utf32(p, n, c);
            p += o;
            n -= o;
            buffer.push_back(c);
        }
        return buffer;
    }

    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u8string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u32string>(std::u8string_view(s));
    }

    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string_view const& s) {
        std::u32string buffer;
        char16_t const* p = s.data();
        size_t n = s.size();
        size_t o{};
        char32_t c{};
        while (n > 0) {
            o = details::utf16_to_utf32(p, n, c);
            p += o;
            n -= o;
            buffer.push_back(c);
        }
        return buffer;
    }

    template<> inline std::u32string PAINFUL_CPP_STRING_CONVERSION_FUNCTION(std::u16string const& s) {
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION<std::u32string>(std::u16string_view(s));
    }
}

#endif
