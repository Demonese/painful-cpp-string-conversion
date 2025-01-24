# Painful C++ string conversion

Why the conversion between C++ string/wstring/u8string/u16string/u32string so painful?

## Conversion matrix

|                    | **std::string** | **std::u8string**  | **std::u16string** | **std::u32string** |  **std::wstring**  |
|:------------------:|:---------------:|:------------------:|:------------------:|:------------------:|:------------------:|
| **std::string**    |                 | :warning:          | :warning:          | :warning:          | :warning:          |
| **std::u8string**  | :warning:       |                    | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **std::u16string** | :warning:       | :white_check_mark: |                    | :white_check_mark: | :white_check_mark: |
| **std::u32string** | :warning:       | :white_check_mark: | :white_check_mark: |                    | :white_check_mark: |
| **std::wstring**   | :warning:       | :white_check_mark: | :white_check_mark: | :white_check_mark: |                    |
