// SPDX-License-Identifier: MIT
// Project: Painful C++ string conversion
// Url: https://github.com/Demonese/painful-cpp-string-conversion
// Author: 璀境石 (https://github.com/Demonese)
// File: ext/convert.hpp
#pragma once

#ifndef PAINFUL_CPP_STRING_CONVERSION_EXT_CONVERT_H
#define PAINFUL_CPP_STRING_CONVERSION_EXT_CONVERT_H

#include <cstddef>
#include <climits>
#include <cstdint>
#include <type_traits>
#include <string_view>

#ifndef PAINFUL_CPP_STRING_CONVERSION_NAMESPACE
#define PAINFUL_CPP_STRING_CONVERSION_NAMESPACE ext
#define PAINFUL_CPP_STRING_CONVERSION_NAMESPACE_DEFAULT
#endif

#ifndef PAINFUL_CPP_STRING_CONVERSION_FUNCTION
#define PAINFUL_CPP_STRING_CONVERSION_FUNCTION convert
#define PAINFUL_CPP_STRING_CONVERSION_FUNCTION_DEFAULT
#endif

namespace PAINFUL_CPP_STRING_CONVERSION_NAMESPACE {
    static_assert(CHAR_BIT == 8, "unsupported char type");

#ifdef __cpp_char8_t
    static_assert(sizeof(char) == sizeof(char8_t), "unsupported char type");
#endif // __cpp_char8_t

#ifdef _WIN32
    static_assert(sizeof(wchar_t) == sizeof(char16_t), "unsupported wchar_t type");
#else // _WIN32
    static_assert(sizeof(wchar_t) == sizeof(char32_t), "unsupported wchar_t type");
#endif // _WIN32

    namespace details {
#ifdef __cpp_char8_t
        template<typename T>
        constexpr bool is_char8_type_v = std::is_same_v<T, char> || std::is_same_v<T, uint8_t> || std::is_same_v<T, char8_t>;
#else // __cpp_char8_t
        template<typename T>
        constexpr bool is_char8_type_v = std::is_same_v<T, char> || std::is_same_v<T, uint8_t>;
#endif // __cpp_char8_t

#ifdef _WIN32
        template<typename T>
        constexpr bool is_char16_type_v = std::is_same_v<T, uint16_t> || std::is_same_v<T, char16_t> || std::is_same_v<T, wchar_t>;
        template<typename T>
        constexpr bool is_char32_type_v = std::is_same_v<T, uint32_t> || std::is_same_v<T, char32_t>;
#else // _WIN32
        template<typename T>
        constexpr bool is_char16_type_v = std::is_same_v<T, uint16_t> || std::is_same_v<T, char16_t>;
        template<typename T>
        constexpr bool is_char32_type_v = std::is_same_v<T, uint32_t> || std::is_same_v<T, char32_t> || std::is_same_v<T, wchar_t>;
#endif // _WIN32

        template<typename T>
        constexpr bool is_supported_char_type_v = is_char8_type_v<T> || is_char16_type_v<T> || is_char32_type_v<T>;

        template<typename T> [[nodiscard]] inline bool is_utf8_1(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return c <= 0x7fu; }
        template<typename T> [[nodiscard]] inline bool is_utf8_2(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return c > 0x7fu && c <= 0x7ffu; }
        template<typename T> [[nodiscard]] inline bool is_utf8_3(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return c > 0x7ffu && c <= 0xffffu; }
        template<typename T> [[nodiscard]] inline bool is_utf8_4(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return c > 0xffffu && c <= 0x1f'ffffu; }
        template<typename T> [[nodiscard]] inline bool is_utf8_5(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return c > 0x1f'ffffu && c <= 0x3ff'ffffu; }
        template<typename T> [[nodiscard]] inline bool is_utf8_6(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return c > 0x3ff'ffffu && c <= 0x7fff'ffffu; }

        template<typename R,        typename C> [[nodiscard]] inline R e_utf8_1(C const c) noexcept { static_assert(is_char8_type_v<R> && (is_char16_type_v<C> || is_char32_type_v<C>), "unsupported type"); return c & 0b0111'1111; }
        template<typename R,        typename C> [[nodiscard]] inline R e_utf8_2(C const c) noexcept { static_assert(is_char8_type_v<R> && (is_char16_type_v<C> || is_char32_type_v<C>), "unsupported type"); return     0b1100'0000 | (((c >> (6 * 1)) & 0b0001'1111)); }
        template<typename R,        typename C> [[nodiscard]] inline R e_utf8_3(C const c) noexcept { static_assert(is_char8_type_v<R> && (is_char16_type_v<C> || is_char32_type_v<C>), "unsupported type"); return     0b1110'0000 | (((c >> (6 * 2)) & 0b0000'1111)); }
        template<typename R,        typename C> [[nodiscard]] inline R e_utf8_4(C const c) noexcept { static_assert(is_char8_type_v<R> && (is_char16_type_v<C> || is_char32_type_v<C>), "unsupported type"); return     0b1111'0000 | (((c >> (6 * 3)) & 0b0000'0111)); }
        template<typename R,        typename C> [[nodiscard]] inline R e_utf8_5(C const c) noexcept { static_assert(is_char8_type_v<R> && (is_char16_type_v<C> || is_char32_type_v<C>), "unsupported type"); return     0b1111'1000 | (((c >> (6 * 4)) & 0b0000'0011)); }
        template<typename R,        typename C> [[nodiscard]] inline R e_utf8_6(C const c) noexcept { static_assert(is_char8_type_v<R> && (is_char16_type_v<C> || is_char32_type_v<C>), "unsupported type"); return     0b1111'1100 | (((c >> (6 * 5)) & 0b0000'0001)); }
        template<typename R, int N, typename C> [[nodiscard]] inline R e_utf8_m(C const c) noexcept { static_assert(is_char8_type_v<R> && (is_char16_type_v<C> || is_char32_type_v<C>), "unsupported type"); return     0b1000'0000 | (((c >> (6 * N)) & 0b0011'1111)); }

        template<typename T> [[nodiscard]] inline bool is_utf16_h(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return (c & 0xfc00u) == 0xd800u; }
        template<typename T> [[nodiscard]] inline bool is_utf16_l(T const c) noexcept { static_assert(is_char16_type_v<T> || is_char32_type_v<T>, "unsupported type"); return (c & 0xfc00u) == 0xdc00u; }

        template<typename T>
        [[nodiscard]] inline size_t utf32_to_utf8(char32_t const c, T (&s)[8], bool const extended = false) noexcept {
            static_assert(is_char8_type_v<T>, "unsupported type");
            if (is_utf8_1(c)) {
                s[0] = e_utf8_1<T>(c);
                s[1] = {};
                return 1;
            }
            if (is_utf8_2(c)) {
                s[0] = e_utf8_2<T>(c);
                s[1] = e_utf8_m<T, 0>(c);
                s[2] = {};
                return 2;
            }
            if (!is_utf16_h(c) && !is_utf16_l(c) && is_utf8_3(c)) {
                s[0] = e_utf8_3<T>(c);
                s[1] = e_utf8_m<T, 1>(c);
                s[2] = e_utf8_m<T, 0>(c);
                s[3] = {};
                return 3;
            }
            if (is_utf8_4(c)) {
                s[0] = e_utf8_4<T>(c);
                s[1] = e_utf8_m<T, 2>(c);
                s[2] = e_utf8_m<T, 1>(c);
                s[3] = e_utf8_m<T, 0>(c);
                s[4] = {};
                return 4;
            }
            if (extended && is_utf8_5(c)) {
                s[0] = e_utf8_5<T>(c);
                s[1] = e_utf8_m<T, 3>(c);
                s[2] = e_utf8_m<T, 2>(c);
                s[3] = e_utf8_m<T, 1>(c);
                s[4] = e_utf8_m<T, 0>(c);
                s[5] = {};
                return 5;
            }
            if (extended && is_utf8_6(c)) {
                s[0] = e_utf8_6<T>(c);
                s[1] = e_utf8_m<T, 4>(c);
                s[2] = e_utf8_m<T, 3>(c);
                s[3] = e_utf8_m<T, 2>(c);
                s[4] = e_utf8_m<T, 1>(c);
                s[5] = e_utf8_m<T, 0>(c);
                s[6] = {};
                return 6;
            }
            s[0] = u8'?';
            s[1] = {};
            return 1;
        }

        template<typename T>        [[nodiscard]] inline bool      r_utf8_1(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b1000'0000) == 0b0000'0000; }
        template<typename T>        [[nodiscard]] inline bool      r_utf8_2(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b1110'0000) == 0b1100'0000; }
        template<typename T>        [[nodiscard]] inline bool      r_utf8_3(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b1111'0000) == 0b1110'0000; }
        template<typename T>        [[nodiscard]] inline bool      r_utf8_4(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b1111'1000) == 0b1111'0000; }
        template<typename T>        [[nodiscard]] inline bool      r_utf8_5(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b1111'1100) == 0b1111'1000; }
        template<typename T>        [[nodiscard]] inline bool      r_utf8_6(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b1111'1110) == 0b1111'1100; }
        template<typename T>        [[nodiscard]] inline bool      r_utf8_m(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b1100'0000) == 0b1000'0000; }

        template<typename T>        [[nodiscard]] inline char32_t  d_utf8_2(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b0001'1111) << (6 * 1); }
        template<typename T>        [[nodiscard]] inline char32_t  d_utf8_3(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b0000'1111) << (6 * 2); }
        template<typename T>        [[nodiscard]] inline char32_t  d_utf8_4(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b0000'0111) << (6 * 3); }
        template<typename T>        [[nodiscard]] inline char32_t  d_utf8_5(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b0000'0011) << (6 * 4); }
        template<typename T>        [[nodiscard]] inline char32_t  d_utf8_6(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b0000'0001) << (6 * 5); }
        template<int N, typename T> [[nodiscard]] inline char32_t  d_utf8_m(T const c) noexcept { static_assert(is_char8_type_v<T>, "unsupported type"); return (c & 0b0011'1111) << (6 * N); }

        template<typename T>
        [[nodiscard]] inline size_t utf8_to_utf32(T const* const s, size_t const n, char32_t& c, bool const extended = false) noexcept {
            static_assert(is_char8_type_v<T>, "unsupported type");
            if (r_utf8_1(s[0])) {
                c = s[0];
                return 1;
            }
            if (n >= 2 && r_utf8_2(s[0]) && r_utf8_m(s[1])) {
                c = d_utf8_2(s[0])
                | d_utf8_m<0>(s[1]);
                return 2;
            }
            if (n >= 3 && r_utf8_3(s[0]) && r_utf8_m(s[1]) && r_utf8_m(s[2])) {
                c = d_utf8_3(s[0])
                | d_utf8_m<1>(s[1])
                | d_utf8_m<0>(s[2]);
                return 3;
            }
            if (n >= 4 && r_utf8_4(s[0]) && r_utf8_m(s[1]) && r_utf8_m(s[2]) && r_utf8_m(s[3])) {
                c = d_utf8_4(s[0])
                | d_utf8_m<2>(s[1])
                | d_utf8_m<1>(s[2])
                | d_utf8_m<0>(s[3]);
                return 4;
            }
            if (extended && n >= 5 && r_utf8_5(s[0]) && r_utf8_m(s[1]) && r_utf8_m(s[2]) && r_utf8_m(s[3]) && r_utf8_m(s[4])) {
                c = d_utf8_5(s[0])
                | d_utf8_m<3>(s[1])
                | d_utf8_m<2>(s[2])
                | d_utf8_m<1>(s[3])
                | d_utf8_m<0>(s[4]);
                return 5;
            }
            if (extended && n >= 6 && r_utf8_6(s[0]) && r_utf8_m(s[1]) && r_utf8_m(s[2]) && r_utf8_m(s[3]) && r_utf8_m(s[4]) && r_utf8_m(s[5])) {
                c = d_utf8_6(s[0])
                | d_utf8_m<4>(s[1])
                | d_utf8_m<3>(s[2])
                | d_utf8_m<2>(s[3])
                | d_utf8_m<1>(s[4])
                | d_utf8_m<0>(s[5]);
                return 6;
            }
            c = U'?';
            return 1;
        }

        template<typename T>
        [[nodiscard]] inline char32_t utf16_to_utf32(T const h, T const l) noexcept {
            static_assert(is_char16_type_v<T>, "unsupported type");
            return 0x10000u + ((h & 0x3ffu) << 10) | (l & 0x3ffu);
        }
        template<typename T>
        [[nodiscard]] inline size_t utf16_to_utf32(T const* const s, size_t const n, char32_t& c) {
            static_assert(is_char16_type_v<T>, "unsupported type");
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

        template<typename T>
        [[nodiscard]] inline size_t utf32_to_utf16(char32_t const c, T (&s)[4]) noexcept {
            static_assert(is_char16_type_v<T>, "unsupported type");
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

    template<typename Target, typename Source>
    Target PAINFUL_CPP_STRING_CONVERSION_FUNCTION(Source const& s, bool extended = false) {
        static_assert(details::is_supported_char_type_v<typename Source::value_type> && details::is_supported_char_type_v<typename Target::value_type>, "unsupported type");
        if constexpr (std::is_same_v<Source, Target>) {
            return s;
        }
        else if constexpr (std::is_same_v<typename Source::value_type, typename Target::value_type>) {
            return { s.data(), s.size() };
        }
        else if constexpr ((details::is_char8_type_v<typename Source::value_type> && details::is_char8_type_v<typename Target::value_type>) || (details::is_char16_type_v<typename Source::value_type> && details::is_char16_type_v<typename Target::value_type>)) {
            return { reinterpret_cast<typename Target::value_type const*>(s.data()), s.size() };
        }
        else {
            typename Source::value_type const* s_ptr = s.data();
            size_t s_len = s.size();
            Target d;
            char32_t c32{};
            while (s_len > 0) {
                // decode
                if constexpr (details::is_char8_type_v<typename Source::value_type>) {
                    auto const n = details::utf8_to_utf32(s_ptr, s_len, c32, extended);
                    s_ptr += n;
                    s_len -= n;
                }
                else if constexpr (details::is_char16_type_v<typename Source::value_type>) {
                    auto const n = details::utf16_to_utf32(s_ptr, s_len, c32);
                    s_ptr += n;
                    s_len -= n;
                }
                else if constexpr (details::is_char32_type_v<typename Source::value_type>) {
                    c32 = s_ptr[0];
                    s_ptr++;
                    s_len--;
                }
                else {
                    static_assert(details::is_supported_char_type_v<typename Source::value_type>, "unsupported type");
                }
                // encode
                if constexpr (details::is_char8_type_v<typename Target::value_type>) {
                    typename Target::value_type c8[8]{};
                    auto const n = details::utf32_to_utf8(c32, c8, extended);
                    d.append(c8, n);
                }
                else if constexpr (details::is_char16_type_v<typename Target::value_type>) {
                    typename Target::value_type c16[4]{};
                    auto const n = details::utf32_to_utf16(c32, c16);
                    d.append(c16, n);
                }
                else if constexpr (details::is_char32_type_v<typename Target::value_type>) {
                    d.push_back(c32);
                }
                else {
                    static_assert(details::is_supported_char_type_v<typename Target::value_type>, "unsupported type");
                }
            }
            return d;
        }
    }

    template<typename Target, typename InputChar, size_t InputLength>
    Target PAINFUL_CPP_STRING_CONVERSION_FUNCTION(InputChar const (&s)[InputLength], bool extended = false) {
        static_assert(InputLength >= 1, "null-terminate is required");
        return PAINFUL_CPP_STRING_CONVERSION_FUNCTION <Target>(std::basic_string_view<InputChar>{ s, InputLength - 1 }, extended);
    }
}

#ifdef PAINFUL_CPP_STRING_CONVERSION_NAMESPACE_DEFAULT
#undef PAINFUL_CPP_STRING_CONVERSION_NAMESPACE
#undef PAINFUL_CPP_STRING_CONVERSION_NAMESPACE_DEFAULT
#endif

#ifdef PAINFUL_CPP_STRING_CONVERSION_FUNCTION_DEFAULT
#undef PAINFUL_CPP_STRING_CONVERSION_FUNCTION
#undef PAINFUL_CPP_STRING_CONVERSION_FUNCTION_DEFAULT
#endif

#endif // PAINFUL_CPP_STRING_CONVERSION_EXT_CONVERT_H
