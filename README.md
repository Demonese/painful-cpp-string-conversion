# Painful C++ string conversion

Why the conversion between C++ string/wstring/u8string/u16string/u32string so painful?

## Usage

You can convert strings through the following API:

* `encoding::to_string`
* `encoding::to_u8string`
* `encoding::to_u16string`
* `encoding::to_u32string`
* `encoding::to_wstring`

> 

```c++
#include <iostream>
#include <encoding/string.hpp>

using std::string_view_literals::operator""sv;

int main() {
  std::wcout << encoding::to_wstring("Hello world!"sv) << std::endl;
  return 0;
}
```

## Conversion matrix

|                    | **std::string** | **std::u8string**  | **std::u16string** | **std::u32string** |  **std::wstring**  |
|:------------------:|:---------------:|:------------------:|:------------------:|:------------------:|:------------------:|
| **std::string**    |                 | :warning:          | :warning:          | :warning:          | :warning:          |
| **std::u8string**  | :warning:       |                    | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **std::u16string** | :warning:       | :white_check_mark: |                    | :white_check_mark: | :white_check_mark: |
| **std::u32string** | :warning:       | :white_check_mark: | :white_check_mark: |                    | :white_check_mark: |
| **std::wstring**   | :warning:       | :white_check_mark: | :white_check_mark: | :white_check_mark: |                    |

* :white_check_mark: OK.
* :warning: Assuming that std::string always stores text in UTF-8 encoding.
* std::wstring can be considered as std::u16string on Windows.

# TODO

* [ ] Supports raw character array as parameter
* [ ] Supports span or span-like types as parameter
* [ ] Clean up code and reduce duplication
* [ ] Supports simdutf as a high-speed conversion implementation
