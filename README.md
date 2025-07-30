# Painful C++ string conversion

[English](./README.md) [中文](./README.ZH.md)

[![CMake on multiple platforms](https://github.com/Demonese/painful-cpp-string-conversion/actions/workflows/cmake-multi-platform.yml/badge.svg)](https://github.com/Demonese/painful-cpp-string-conversion/actions/workflows/cmake-multi-platform.yml)

Why the conversion between C++ string/wstring/u8string/u16string/u32string so painful?

## Usage

You can convert strings through the following API:

* `ext::convert<std::string>`
* `ext::convert<std::u8string>`
* `ext::convert<std::u16string>`
* `ext::convert<std::u32string>`
* `ext::convert<std::wstring>`

```c++
#include <iostream>
#include <ext/convert.hpp>

using std::string_view_literals::operator""sv;

int main() {
  std::cout << ext::convert<std::string>(U"Hello world!"sv) << std::endl;
  return 0;
}
```

You can customize namespace and function name:

```c++
#include <iostream>
#define PAINFUL_CPP_STRING_CONVERSION_NAMESPACE utf
#define PAINFUL_CPP_STRING_CONVERSION_FUNCTION  as
#include <ext/convert.hpp>

using std::string_view_literals::operator""sv;

int main() {
  std::cout << utf::as<std::string>(U"Hello world!"sv) << std::endl;
  return 0;
}
```

## Conversion matrix

|                    | **std::string** | **std::u8string**  | **std::u16string** | **std::u32string** |  **std::wstring**  |
|:------------------:|:---------------:|:------------------:|:------------------:|:------------------:|:------------------:|
|  **std::string**   |                 |     :warning:      |     :warning:      |     :warning:      |     :warning:      |
| **std::u8string**  |    :warning:    |                    | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **std::u16string** |    :warning:    | :white_check_mark: |                    | :white_check_mark: | :white_check_mark: |
| **std::u32string** |    :warning:    | :white_check_mark: | :white_check_mark: |                    | :white_check_mark: |
|  **std::wstring**  |    :warning:    | :white_check_mark: | :white_check_mark: | :white_check_mark: |                    |

|                        | **std::string_view** | **std::u8string_view** |
|:----------------------:|:--------------------:|:----------------------:|
|  **std::string_view**  |                      |       :warning:        |
| **std::u8string_view** |      :warning:       |                        |

* :white_check_mark: OK.
* :warning: Assuming that std::string always stores text in UTF-8 encoding.
* std::wstring can be considered as std::u16string on Windows.

# TODO

* [ ] Supports span or span-like types as parameter
* [ ] Supports simdutf as a high-speed conversion implementation
