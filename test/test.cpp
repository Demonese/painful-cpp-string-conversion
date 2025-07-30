#include "ext/convert.hpp"
#include "simdutf.h"

#include <cassert>
#include <cstdio>
#include <stdexcept>
#include <string_view>
#include <string>

// c++ 17 polyfill
#ifndef __cpp_char8_t
using char8_t = uint8_t;
#endif // __cpp_char8_t
#ifndef __cpp_lib_char8_t
namespace std {
    using u8string = basic_string<char8_t>;
    using u8string_view = basic_string_view<char8_t>;
}
#endif // __cpp_lib_char8_t

namespace {
    void assert_true(bool const condition) {
        if (condition) {
            return;
        }
        assert(false);
        throw std::runtime_error("failed");
    }
}

int main() {
    for (char32_t c = 0; c <= 0x10'ffff; c++) {
        if (ext::details::is_utf16_h(c) || ext::details::is_utf16_l(c)) {
            continue;
        }

        std::u32string_view const source(&c, 1);

        auto const intermediate_string = ext::convert<std::string>(source);
        auto const intermediate_string_utf8 = ext::convert<std::u8string>(source);
        auto const intermediate_string_utf16 = ext::convert<std::u16string>(source);
        std::u32string const intermediate_string_utf32(source);
    #ifdef _WIN32
        auto const intermediate_string_wide = ext::convert<std::wstring>(source);
    #endif

        assert_true(std::memcmp(intermediate_string.data(), intermediate_string_utf8.data(), sizeof(char8_t) * intermediate_string_utf8.size()) == 0);
    #ifdef _WIN32
        assert_true(std::memcmp(intermediate_string_wide.data(), intermediate_string_utf16.data(), sizeof(char16_t) * intermediate_string_utf16.size()) == 0);
    #endif

        std::string_view const intermediate_string_view(intermediate_string);
        std::u8string_view const intermediate_string_view_utf8(intermediate_string_utf8);
        std::u16string_view const intermediate_string_view_utf16(intermediate_string_utf16);
        std::u32string_view const intermediate_string_view_utf32(intermediate_string_utf32);
        #ifdef _WIN32
        std::wstring_view const intermediate_string_view_wide(intermediate_string_wide);
        #endif

        auto const group11_string_utf8 = ext::convert<std::u8string>(intermediate_string_view);
        auto const group11_string_utf16 = ext::convert<std::u16string>(intermediate_string_view);
        auto const group11_string_utf32 = ext::convert<std::u32string>(intermediate_string_view);
        #ifdef _WIN32
        auto const group11_string_wide = ext::convert<std::wstring>(intermediate_string_view);
        #endif

        auto const group12_string_utf8 = ext::convert<std::u8string>(intermediate_string);
        auto const group12_string_utf16 = ext::convert<std::u16string>(intermediate_string);
        auto const group12_string_utf32 = ext::convert<std::u32string>(intermediate_string);
        #ifdef _WIN32
        auto const group12_string_wide = ext::convert<std::wstring>(intermediate_string);
        #endif

        auto const group21_string = ext::convert<std::string>(intermediate_string_view_utf8);
        auto const group21_string_utf16 = ext::convert<std::u16string>(intermediate_string_view_utf8);
        auto const group21_string_utf32 = ext::convert<std::u32string>(intermediate_string_view_utf8);
        #ifdef _WIN32
        auto const group21_string_wide = ext::convert<std::wstring>(intermediate_string_view_utf8);
        #endif

        auto const group22_string = ext::convert<std::string>(intermediate_string_utf8);
        auto const group22_string_utf16 = ext::convert<std::u16string>(intermediate_string_utf8);
        auto const group22_string_utf32 = ext::convert<std::u32string>(intermediate_string_utf8);
        #ifdef _WIN32
        auto const group22_string_wide = ext::convert<std::wstring>(intermediate_string_utf8);
        #endif

        auto const group31_string = ext::convert<std::string>(intermediate_string_view_utf16);
        auto const group31_string_utf8 = ext::convert<std::u8string>(intermediate_string_view_utf16);
        auto const group31_string_utf32 = ext::convert<std::u32string>(intermediate_string_view_utf16);
        #ifdef _WIN32
        auto const group31_string_wide = ext::convert<std::wstring>(intermediate_string_view_utf16);
        #endif

        auto const group32_string = ext::convert<std::string>(intermediate_string_utf16);
        auto const group32_string_utf8 = ext::convert<std::u8string>(intermediate_string_utf16);
        auto const group32_string_utf32 = ext::convert<std::u32string>(intermediate_string_utf16);
        #ifdef _WIN32
        auto const group32_string_wide = ext::convert<std::wstring>(intermediate_string_utf16);
        #endif

        auto const group41_string = ext::convert<std::string>(intermediate_string_view_utf32);
        auto const group41_string_utf8 = ext::convert<std::u8string>(intermediate_string_view_utf32);
        auto const group41_string_utf16 = ext::convert<std::u16string>(intermediate_string_view_utf32);
        #ifdef _WIN32
        auto const group41_string_wide = ext::convert<std::wstring>(intermediate_string_view_utf32);
        #endif

        auto const group42_string = ext::convert<std::string>(intermediate_string_utf32);
        auto const group42_string_utf8 = ext::convert<std::u8string>(intermediate_string_utf32);
        auto const group42_string_utf16 = ext::convert<std::u16string>(intermediate_string_utf32);
        #ifdef _WIN32
        auto const group42_string_wide = ext::convert<std::wstring>(intermediate_string_utf32);
        #endif

        #ifdef _WIN32
        auto const group51_string = ext::convert<std::string>(intermediate_string_view_wide);
        auto const group51_string_utf8 = ext::convert<std::u8string>(intermediate_string_view_wide);
        auto const group51_string_utf16 = ext::convert<std::u16string>(intermediate_string_view_wide);
        auto const group51_string_utf32 = ext::convert<std::u32string>(intermediate_string_view_wide);

        auto const group52_string = ext::convert<std::string>(intermediate_string_wide);
        auto const group52_string_utf8 = ext::convert<std::u8string>(intermediate_string_wide);
        auto const group52_string_utf16 = ext::convert<std::u16string>(intermediate_string_wide);
        auto const group52_string_utf32 = ext::convert<std::u32string>(intermediate_string_wide);
        #endif

        assert_true(intermediate_string == intermediate_string_view);
        assert_true(intermediate_string == group21_string);
        assert_true(intermediate_string == group22_string);
        assert_true(intermediate_string == group31_string);
        assert_true(intermediate_string == group32_string);
        assert_true(intermediate_string == group41_string);
        assert_true(intermediate_string == group42_string);
        #ifdef _WIN32
        assert_true(intermediate_string == group51_string);
        assert_true(intermediate_string == group52_string);
        #endif

        assert_true(intermediate_string_utf8 == intermediate_string_view_utf8);
        assert_true(intermediate_string_utf8 == group11_string_utf8);
        assert_true(intermediate_string_utf8 == group12_string_utf8);
        assert_true(intermediate_string_utf8 == group31_string_utf8);
        assert_true(intermediate_string_utf8 == group32_string_utf8);
        assert_true(intermediate_string_utf8 == group41_string_utf8);
        assert_true(intermediate_string_utf8 == group42_string_utf8);
        #ifdef _WIN32
        assert_true(intermediate_string_utf8 == group51_string_utf8);
        assert_true(intermediate_string_utf8 == group52_string_utf8);
        #endif

        assert_true(intermediate_string_utf16 == intermediate_string_view_utf16);
        assert_true(intermediate_string_utf16 == group11_string_utf16);
        assert_true(intermediate_string_utf16 == group12_string_utf16);
        assert_true(intermediate_string_utf16 == group21_string_utf16);
        assert_true(intermediate_string_utf16 == group22_string_utf16);
        assert_true(intermediate_string_utf16 == group41_string_utf16);
        assert_true(intermediate_string_utf16 == group42_string_utf16);
        #ifdef _WIN32
        assert_true(intermediate_string_utf16 == group51_string_utf16);
        assert_true(intermediate_string_utf16 == group52_string_utf16);
        #endif

        assert_true(intermediate_string_utf32 == intermediate_string_view_utf32);
        assert_true(intermediate_string_utf32 == group11_string_utf32);
        assert_true(intermediate_string_utf32 == group12_string_utf32);
        assert_true(intermediate_string_utf32 == group21_string_utf32);
        assert_true(intermediate_string_utf32 == group22_string_utf32);
        assert_true(intermediate_string_utf32 == group31_string_utf32);
        assert_true(intermediate_string_utf32 == group32_string_utf32);
        #ifdef _WIN32
        assert_true(intermediate_string_utf32 == group51_string_utf32);
        assert_true(intermediate_string_utf32 == group52_string_utf32);
        #endif

        #ifdef _WIN32
        assert_true(intermediate_string_wide == intermediate_string_view_wide);
        assert_true(intermediate_string_wide == group11_string_wide);
        assert_true(intermediate_string_wide == group12_string_wide);
        assert_true(intermediate_string_wide == group21_string_wide);
        assert_true(intermediate_string_wide == group22_string_wide);
        assert_true(intermediate_string_wide == group31_string_wide);
        assert_true(intermediate_string_wide == group32_string_wide);
        assert_true(intermediate_string_wide == group41_string_wide);
        assert_true(intermediate_string_wide == group42_string_wide);
        #endif

        char string_data[8]{};
        auto const string_size = simdutf::convert_valid_utf32_to_utf8(&c, 1, string_data);

        char16_t string_data_utf16[4]{};
        auto const string_size_utf16 = simdutf::convert_valid_utf32_to_utf16(&c, 1, string_data_utf16);

        assert_true(intermediate_string == std::string_view(string_data, string_size));
        assert_true(intermediate_string_utf16 == std::u16string_view(string_data_utf16, string_size_utf16));
    }
    std::printf("All test pass.\n");
    return 0;
}
