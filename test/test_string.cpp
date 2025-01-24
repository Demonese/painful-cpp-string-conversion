#include "encoding/string.hpp"
#include "simdutf.h"

#include <iostream>
#include <format>
#include <stdexcept>

namespace {
    void rt_assert_true(bool const condition) {
        if (condition) {
            return;
        }
        throw std::runtime_error("error");
    }
}

int main() {
    for (char32_t c = 0; c <= 0x10'ffff; c++) {
        auto const s = encoding::to_string(std::u32string_view(&c, 1));
        auto const u8 = encoding::to_u8string(std::u32string_view(&c, 1));
        auto const u16 = encoding::to_u16string(std::u32string_view(&c, 1));
        auto const w = encoding::to_wstring(std::u32string_view(&c, 1));

        char c_s[8]{};
        size_t c_s_n = simdutf::convert_valid_utf32_to_utf8(&c, 1, c_s);

        char16_t u16_s[4]{};
        size_t u16_s_n = simdutf::convert_valid_utf32_to_utf16(&c, 1, u16_s);

        rt_assert_true(std::string_view(c_s, c_s_n) == s);
        rt_assert_true(std::u16string_view(u16_s, u16_s_n) == u16);
    }
    return 0;
}
