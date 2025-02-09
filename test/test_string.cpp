#include "encoding/string.hpp"
#include "simdutf.h"

#include <format>
#include <stdexcept>

namespace {
    using std::string_view_literals::operator""sv;

    void rt_assert_true(bool const condition) {
        if (condition) {
            return;
        }
        throw std::runtime_error("failed");
    }
}

int main() {
    for (char32_t c = 0; c <= 0x10'ffff; c++) {
        auto const s = ext::to_string(std::u32string_view(&c, 1));
        auto const u8 = ext::to_u8string(std::u32string_view(&c, 1));
        auto const u16 = ext::to_u16string(std::u32string_view(&c, 1));
        auto const w = ext::to_wstring(std::u32string_view(&c, 1));

        char c_s[8]{};
        size_t c_s_n = simdutf::convert_valid_utf32_to_utf8(&c, 1, c_s);

        char16_t u16_s[4]{};
        size_t u16_s_n = simdutf::convert_valid_utf32_to_utf16(&c, 1, u16_s);

        rt_assert_true(std::memcmp(s.data(), u8.data(), sizeof(char8_t) * s.size()) == 0);
        rt_assert_true(std::memcmp(w.data(), u16.data(), sizeof(char16_t) * s.size()) == 0);
        if (ext::details::is_utf16_h(c) || ext::details::is_utf16_l(c)) {
            rt_assert_true(std::string_view(c_s, c_s_n) == ""sv);
            rt_assert_true(std::u16string_view(u16_s, u16_s_n) == u""sv);
            rt_assert_true("?"sv == s);
            rt_assert_true(u"?"sv == u16);
        }
        else {
            rt_assert_true(std::string_view(c_s, c_s_n) == s);
            rt_assert_true(std::u16string_view(u16_s, u16_s_n) == u16);
        }
    }
    return 0;
}
