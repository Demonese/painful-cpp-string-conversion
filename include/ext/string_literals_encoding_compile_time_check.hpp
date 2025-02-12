// SPDX-License-Identifier: MIT
// Project: Painful C++ string conversion
// Url: https://github.com/Demonese/painful-cpp-string-conversion
// Author: 璀境石 (https://github.com/Demonese)
// File: ext/string_literals_encoding_compile_time_check.hpp
#pragma once

#ifndef PAINFUL_CPP_STRING_CONVERSION_EXT_STRING_LITERALS_ENCODING_COMPILE_TIME_CHECK_H
#define PAINFUL_CPP_STRING_CONVERSION_EXT_STRING_LITERALS_ENCODING_COMPILE_TIME_CHECK_H

#include <climits>

static_assert(CHAR_BIT == 8);

static_assert(sizeof(char) == sizeof(char8_t));

static_assert(sizeof("Ω") == 3 && ("Ω")[0] == '\xCE' && ("Ω")[1] == '\xA9');
static_assert(sizeof("永") == 4 && ("永")[0] == '\xE6' && ("永")[1] == '\xB0' && ("永")[2] == '\xB8');
static_assert(sizeof("🙂") == 5 && ("🙂")[0] == '\xF0' && ("🙂")[1] == '\x9F' && ("🙂")[2] == '\x99' && ("🙂")[3] == '\x82');

#endif
