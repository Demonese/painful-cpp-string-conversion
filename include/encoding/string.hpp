// SPDX-License-Identifier: MIT
// Author: 璀境石 (GitHub: @Demonese / @chuikingshek)
#pragma once

#ifndef PAINFUL_CPP_STRING_CONVERSION_ENCODING_STRING_H
#define PAINFUL_CPP_STRING_CONVERSION_ENCODING_STRING_H

#include <string>
#include <string_view>

namespace encoding {
    // Does the compiler parse the source file with the correct encoding (utf-8) ?
    // If you are using MSVC compiler, please remember to add /utf-8 compile option.
    static_assert(sizeof("Ω") == 3 && ("Ω")[0] == '\xCE' && ("Ω")[1] == '\xA9');
    static_assert(sizeof("永") == 4 && ("永")[0] == '\xE6' && ("永")[1] == '\xB0' && ("永")[2] == '\xB8');
    // The C++ compiler you are using should support UTF-8 4-bytes
    static_assert(sizeof("🙂") == 5 && ("🙂")[0] == '\xF0' && ("🙂")[1] == '\x9F' && ("🙂")[2] == '\x99' && ("🙂")[3] == '\x82');

    // We assume that char has 8 bits
    static_assert(CHAR_BIT == 8);
#ifdef __cpp_lib_char8_t
    static_assert(sizeof(char) == sizeof(char8_t));
#endif

    namespace details {
        [[nodiscard]] inline bool is_utf8_1(char32_t const c) noexcept { return c <= 0x7f; }
        [[nodiscard]] inline bool is_utf8_2(char32_t const c) noexcept { return c <= 0x7ff; }
        [[nodiscard]] inline bool is_utf8_3(char32_t const c) noexcept { return c <= 0xffff; }
        [[nodiscard]] inline bool is_utf8_4(char32_t const c) noexcept { return c <= 0x1f'ffff; }
        [[nodiscard]] inline bool is_utf8_5(char32_t const c) noexcept { return c <= 0x3ff'ffff; }
        [[nodiscard]] inline bool is_utf8_6(char32_t const c) noexcept { return c <= 0x7fff'ffff; }

        [[nodiscard]] inline char8_t e_utf8_1(char32_t const c) noexcept { return c & 0x7fui8; }
        [[nodiscard]] inline char8_t e_utf8_2(char32_t const c) noexcept { return 0b11000000ui8 | (((c >> (6 * 1)) & 0b00011111ui8)); }
        [[nodiscard]] inline char8_t e_utf8_3(char32_t const c) noexcept { return 0b11100000ui8 | (((c >> (6 * 2)) & 0b00001111ui8)); }
        [[nodiscard]] inline char8_t e_utf8_4(char32_t const c) noexcept { return 0b11110000ui8 | (((c >> (6 * 3)) & 0b00000111ui8)); }
        [[nodiscard]] inline char8_t e_utf8_5(char32_t const c) noexcept { return 0b11111000ui8 | (((c >> (6 * 4)) & 0b00000011ui8)); }
        [[nodiscard]] inline char8_t e_utf8_6(char32_t const c) noexcept { return 0b11111100ui8 | (((c >> (6 * 5)) & 0b00000001ui8)); }
        [[nodiscard]] inline char8_t e_utf8_m(char32_t const c, int shift) noexcept { return 0x80ui8 | ((c >> shift) & 0x3fui8); }

        [[nodiscard]] inline size_t utf32_to_utf8(char32_t const c, char8_t (&s)[8], bool const extended = false) noexcept {
            if (is_utf8_1(c)) {
                s[0] = e_utf8_1(c);
                s[1] = 0;
                return 1;
            }
            else if (is_utf8_2(c)) {
                s[0] = e_utf8_2(c);
                s[1] = e_utf8_m(c, 6 * 0);
                s[2] = 0;
                return 2;
            }
            else if (is_utf8_3(c)) {
                s[0] = e_utf8_3(c);
                s[1] = e_utf8_m(c, 6 * 1);
                s[2] = e_utf8_m(c, 6 * 0);
                s[3] = 0;
                return 3;
            }
            else if (is_utf8_4(c)) {
                s[0] = e_utf8_4(c);
                s[1] = e_utf8_m(c, 6 * 2);
                s[2] = e_utf8_m(c, 6 * 1);
                s[3] = e_utf8_m(c, 6 * 0);
                s[4] = 0;
                return 4;
            }
            else if (extended && is_utf8_5(c)) {
                s[0] = e_utf8_5(c);
                s[1] = e_utf8_m(c, 6 * 3);
                s[2] = e_utf8_m(c, 6 * 2);
                s[3] = e_utf8_m(c, 6 * 1);
                s[4] = e_utf8_m(c, 6 * 0);
                s[5] = 0;
                return 5;
            }
            else if (extended && is_utf8_6(c)) {
                s[0] = e_utf8_6(c);
                s[1] = e_utf8_m(c, 6 * 4);
                s[2] = e_utf8_m(c, 6 * 3);
                s[3] = e_utf8_m(c, 6 * 2);
                s[4] = e_utf8_m(c, 6 * 1);
                s[5] = e_utf8_m(c, 6 * 0);
                s[6] = 0;
                return 6;
            }
            else {
                s[0] = '?';
                s[1] = 0;
                return 1;
            }
        }

        [[nodiscard]] inline bool is_utf8_1(char8_t const c) noexcept { return (c & 0x80ui8) == 0x00ui8; }
        [[nodiscard]] inline bool is_utf8_2(char8_t const c) noexcept { return (c & 0xe0ui8) == 0xc0ui8; }
        [[nodiscard]] inline bool is_utf8_3(char8_t const c) noexcept { return (c & 0xf0ui8) == 0xe0ui8; }
        [[nodiscard]] inline bool is_utf8_4(char8_t const c) noexcept { return (c & 0xf8ui8) == 0xf0ui8; }
        [[nodiscard]] inline bool is_utf8_5(char8_t const c) noexcept { return (c & 0xfcui8) == 0xf8ui8; }
        [[nodiscard]] inline bool is_utf8_6(char8_t const c) noexcept { return (c & 0xfeui8) == 0xfcui8; }
        [[nodiscard]] inline bool is_utf8_m(char8_t const c) noexcept { return (c & 0xc0ui8) == 0x80ui8; }

        [[nodiscard]] inline char32_t d_utf8_2(char8_t const c) noexcept { return (c & 0x1fui32) << 6; }
        [[nodiscard]] inline char32_t d_utf8_3(char8_t const c) noexcept { return (c & 0x0fui32) << 12; }
        [[nodiscard]] inline char32_t d_utf8_4(char8_t const c) noexcept { return (c & 0x07ui32) << 18; }
        [[nodiscard]] inline char32_t d_utf8_5(char8_t const c) noexcept { return (c & 0x03ui32) << 24; }
        [[nodiscard]] inline char32_t d_utf8_6(char8_t const c) noexcept { return (c & 0x01ui32) << 30; }
        [[nodiscard]] inline char32_t d_utf8_m(char8_t const c, int shift) noexcept { return (c & 0x3fui32) << shift; }

        [[nodiscard]] inline size_t utf8_to_utf32(char8_t const* const s, size_t const n, char32_t& c, bool const extended = false) noexcept {
            if (is_utf8_1(s[0])) {
                c = s[0];
                return 1;
            }
            else if (n >= 2 && is_utf8_2(s[0])) {
                if (is_utf8_m(s[1])) {
                    c = d_utf8_2(s[0]) | d_utf8_m(s[1], 0);
                    return 2;
                }
            }
            else if (n >= 3 && is_utf8_3(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2])) {
                    c = d_utf8_3(s[0]) | d_utf8_m(s[1], 6) | d_utf8_m(s[2], 0);
                    return 3;
                }
            }
            else if (n >= 4 && is_utf8_4(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2]) && is_utf8_m(s[3])) {
                    c = d_utf8_4(s[0]) | d_utf8_m(s[1], 12) | d_utf8_m(s[2], 6) | d_utf8_m(s[3], 0);
                    return 4;
                }
            }
            else if (extended && n >= 5 && is_utf8_5(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2]) && is_utf8_m(s[3]) && is_utf8_m(s[4])) {
                    c = d_utf8_5(s[0]) | d_utf8_m(s[1], 18) | d_utf8_m(s[2], 12) | d_utf8_m(s[3], 6) | d_utf8_m(s[4], 0);
                    return 5;
                }
            }
            else if (extended && n >= 6 && is_utf8_6(s[0])) {
                if (is_utf8_m(s[1]) && is_utf8_m(s[2]) && is_utf8_m(s[3]) && is_utf8_m(s[4]) && is_utf8_m(s[5])) {
                    c = d_utf8_6(s[0]) | d_utf8_m(s[1], 24) | d_utf8_m(s[2], 18) | d_utf8_m(s[3], 12) | d_utf8_m(s[4], 6) | d_utf8_m(s[5], 0);
                    return 6;
                }
            }
            c = U'?';
            return 1;
        }

        [[nodiscard]] inline bool is_utf16_h(char16_t const c) noexcept { return (c & 0xfc00ui16) == 0xd800ui16; }
        [[nodiscard]] inline bool is_utf16_l(char16_t const c) noexcept { return (c & 0xfc00ui16) == 0xdc00ui16; }

        [[nodiscard]] inline char32_t utf16_to_utf32(char16_t const h, char16_t const l) noexcept {
            return 0x10000ui32 + ((h & 0x3ffui32) << 10) | (l & 0x3ffui32);
        }

        [[nodiscard]] inline size_t utf16_to_utf32(char16_t const* const s, size_t const n, char32_t& c) {
            if (n >= 2 && is_utf16_h(s[0])) {
                if (is_utf16_l(s[1])) {
                    c = utf16_to_utf32(s[0], s[1]);
                    return 2;
                }
            }
            else if (!is_utf16_l(s[0])) {
                c = s[0];
                return 1;
            }
            c = U'?';
            return 1;
        }

        [[nodiscard]] inline size_t utf32_to_utf16(char32_t const c, char16_t (&s)[4]) noexcept {
            if (c <= 0xffffui32) {
                s[0] = c & 0xffffui16;
                s[1] = 0;
                return 1;
            }
            else if (c <= 0x10'ffffui32) {
                char32_t const d = c - 0x1'0000ui32;
                s[0] = 0xd800ui16 | ((d >> 10) & 0x3ffui16);
                s[1] = 0xdc00ui16 | (d & 0x3ffui16);
                s[2] = 0;
                return 2;
            }
            else {
                s[0] = u'?';
                s[1] = 0;
                return 1;
            }
        }
    }

    // std::string

#ifdef __cpp_lib_char8_t
    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::u8string_view const& s) {
        return { reinterpret_cast<char const*>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::u8string const& s) {
        return { reinterpret_cast<char const*>(s.c_str()), s.length() };
    }
#endif

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::u16string_view const& s) {
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
            buffer.append(reinterpret_cast<char*>(t), m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::u16string const& s) {
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
            buffer.append(reinterpret_cast<char*>(t), m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::u32string_view const& s) {
        std::string buffer;
        char8_t t[8]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf8(c, t);
            buffer.append(reinterpret_cast<char*>(t), m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::u32string const& s) {
        std::string buffer;
        char8_t t[8]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf8(c, t);
            buffer.append(reinterpret_cast<char*>(t), m);
        }
        return buffer;
    }

    // std::u8string

#ifdef __cpp_lib_char8_t
    // Always assume that the std::string stores text encoded in UTF-8
    inline std::u8string to_u8string(std::string_view const& s) {
        return { reinterpret_cast<std::u8string::const_pointer>(s.data()), s.size() };
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::u8string to_u8string(std::string const& s) {
        return { reinterpret_cast<std::u8string::const_pointer>(s.c_str()), s.length() };
    }

    inline std::u8string to_u8string(std::u16string_view const& s) {
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

    inline std::u8string to_u8string(std::u16string const& s) {
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

    inline std::u8string to_u8string(std::u32string_view const& s) {
        std::u8string buffer;
        char8_t t[8]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf8(c, t);
            buffer.append(t, m);
        }
        return buffer;
    }

    inline std::u8string to_u8string(std::u32string const& s) {
        std::u8string buffer;
        char8_t t[8]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf8(c, t);
            buffer.append(t, m);
        }
        return buffer;
    }
#endif

    // std::u16string

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::u16string to_u16string(std::string_view const& s) {
        std::u16string buffer;
        char8_t const* p = reinterpret_cast<char8_t const*>(s.data());
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
    inline std::u16string to_u16string(std::string const& s) {
        std::u16string buffer;
        char8_t const* p = reinterpret_cast<char8_t const*>(s.data());
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

    inline std::u16string to_u16string(std::u8string_view const& s) {
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

    inline std::u16string to_u16string(std::u8string const& s) {
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

    inline std::u16string to_u16string(std::u32string_view const& s) {
        std::u16string buffer;
        char16_t t[4]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf16(c, t);
            buffer.append(t, m);
        }
        return buffer;
    }

    inline std::u16string to_u16string(std::u32string const& s) {
        std::u16string buffer;
        char16_t t[4]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf16(c, t);
            buffer.append(t, m);
        }
        return buffer;
    }

    // std::u32string

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::u32string to_u32string(std::string_view const& s) {
        std::u32string buffer;
        char8_t const* p = reinterpret_cast<char8_t const*>(s.data());
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
    inline std::u32string to_u32string(std::string const& s) {
        std::u32string buffer;
        char8_t const* p = reinterpret_cast<char8_t const*>(s.data());
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

    inline std::u32string to_u32string(std::u8string_view const& s) {
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

    inline std::u32string to_u32string(std::u8string const& s) {
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

    inline std::u32string to_u32string(std::u16string_view const& s) {
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

    inline std::u32string to_u32string(std::u16string const& s) {
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

    // std::wstring

#if WCHAR_MAX == UINT32_MAX
    static_assert(sizeof(wchar_t) == sizeof(char32_t));

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::wstring_view const& s) {
        return to_string(std::u32string_view(reinterpret_cast<char32_t const*>(s.data()), s.size()));
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::wstring const& s) {
        return to_string(std::u32string_view(reinterpret_cast<char32_t const*>(s.data()), s.size()));
    }

#ifdef __cpp_lib_char8_t
    inline std::u8string to_u8string(std::wstring_view const& s) {
        return to_u8string(std::u32string_view(reinterpret_cast<char32_t const*>(s.data()), s.size()));
    }

    inline std::u8string to_u8string(std::wstring const& s) {
        return to_u8string(std::u32string_view(reinterpret_cast<char32_t const*>(s.data()), s.size()));
    }
#endif

    inline std::u16string to_u16string(std::wstring_view const& s) {
        return to_u16string(std::u32string_view(reinterpret_cast<char32_t const*>(s.data()), s.size()));
    }

    inline std::u16string to_u16string(std::wstring const& s) {
        return to_u16string(std::u32string_view(reinterpret_cast<char32_t const*>(s.data()), s.size()));
    }

    inline std::u32string to_u32string(std::wstring_view const& s) {
        return { reinterpret_cast<char32_t const*>(s.data()), s.size() };
    }

    inline std::u32string to_u32string(std::wstring const& s) {
        return { reinterpret_cast<char32_t const*>(s.data()), s.size() };
    }

#elif WCHAR_MAX == UINT16_MAX
    static_assert(sizeof(wchar_t) == sizeof(char16_t));

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::wstring_view const& s) {
        return to_string(std::u16string_view(reinterpret_cast<char16_t const*>(s.data()), s.size()));
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::string to_string(std::wstring const& s) {
        return to_string(std::u16string_view(reinterpret_cast<char16_t const*>(s.data()), s.size()));
    }

#ifdef __cpp_lib_char8_t
    inline std::u8string to_u8string(std::wstring_view const& s) {
        return to_u8string(std::u16string_view(reinterpret_cast<char16_t const*>(s.data()), s.size()));
    }

    inline std::u8string to_u8string(std::wstring const& s) {
        return to_u8string(std::u16string_view(reinterpret_cast<char16_t const*>(s.data()), s.size()));
    }
#endif

    inline std::u16string to_u16string(std::wstring_view const& s) {
        return { reinterpret_cast<char16_t const*>(s.data()), s.size() };
    }

    inline std::u16string to_u16string(std::wstring const& s) {
        return { reinterpret_cast<char16_t const*>(s.data()), s.size() };
    }

    inline std::u32string to_u32string(std::wstring_view const& s) {
        return to_u32string(std::u16string_view(reinterpret_cast<char16_t const*>(s.data()), s.size()));
    }

    inline std::u32string to_u32string(std::wstring const& s) {
        return to_u32string(std::u16string_view(reinterpret_cast<char16_t const*>(s.data()), s.size()));
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::wstring to_wstring(std::string const& s) {
        std::wstring buffer;
        char8_t const* p = reinterpret_cast<char8_t const*>(s.data());
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
            buffer.append(reinterpret_cast<wchar_t const*>(t), m);
        }
        return buffer;
    }

    // Always assume that the std::string stores text encoded in UTF-8
    inline std::wstring to_wstring(std::string_view const& s) {
        std::wstring buffer;
        char8_t const* p = reinterpret_cast<char8_t const*>(s.data());
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
            buffer.append(reinterpret_cast<wchar_t const*>(t), m);
        }
        return buffer;
    }

    inline std::wstring to_wstring(std::u8string_view const& s) {
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
            buffer.append(reinterpret_cast<wchar_t const*>(t), m);
        }
        return buffer;
    }

    inline std::wstring to_wstring(std::u8string const& s) {
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
            buffer.append(reinterpret_cast<wchar_t const*>(t), m);
        }
        return buffer;
    }

    inline std::wstring to_wstring(std::u16string_view const& s) {
        return { reinterpret_cast<std::wstring::const_pointer>(s.data()), s.size() };
    }

    inline std::wstring to_wstring(std::u16string const& s) {
        return { reinterpret_cast<std::wstring::const_pointer>(s.data()), s.size() };
    }

    inline std::wstring to_wstring(std::u32string_view const& s) {
        std::wstring buffer;
        char16_t t[4]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf16(c, t);
            buffer.append(reinterpret_cast<wchar_t const*>(t), m);
        }
        return buffer;
    }

    inline std::wstring to_wstring(std::u32string const& s) {
        std::wstring buffer;
        char16_t t[4]{};
        size_t m{};
        for (auto const c : s) {
            m = details::utf32_to_utf16(c, t);
            buffer.append(reinterpret_cast<wchar_t const*>(t), m);
        }
        return buffer;
    }
#endif
}

#endif
