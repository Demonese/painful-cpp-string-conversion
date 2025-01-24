# 痛苦的 C++ 字符串转换

| [English](./README.md) | [中文](./README.ZH.md) |

为什么 C++ string/wstring/u8string/u16string/u32string 之间的转换这么痛苦？

## 用法

你可以通过以下 API 转换字符串：

* `encoding::to_string`
* `encoding::to_u8string`
* `encoding::to_u16string`
* `encoding::to_u32string`
* `encoding::to_wstring`

```c++
#include <iostream>
#include <encoding/string.hpp>

using std::string_view_literals::operator""sv;

int main() {
  std::wcout << encoding::to_wstring("Hello world!"sv) << std::endl;
  return 0;
}
```

## 互转表格

|                    | **std::string** | **std::u8string**  | **std::u16string** | **std::u32string** |  **std::wstring**  |
|:------------------:|:---------------:|:------------------:|:------------------:|:------------------:|:------------------:|
| **std::string**    |                 | :warning:          | :warning:          | :warning:          | :warning:          |
| **std::u8string**  | :warning:       |                    | :white_check_mark: | :white_check_mark: | :white_check_mark: |
| **std::u16string** | :warning:       | :white_check_mark: |                    | :white_check_mark: | :white_check_mark: |
| **std::u32string** | :warning:       | :white_check_mark: | :white_check_mark: |                    | :white_check_mark: |
| **std::wstring**   | :warning:       | :white_check_mark: | :white_check_mark: | :white_check_mark: |                    |

* :white_check_mark: 可以转换。
* :warning: 始终假设 std::string 以 UTF-8 编码储存文本。
* Windows 的 std::wstring 可以看作 std::u16string。

# 待办事项

* [ ] 支持原始字符数组作为参数
* [ ] 支持 span 或者类似 span 的类型作为参数
* [ ] 清理代码减少冗余重复代码
* [ ] 支持使用 simdutf 库执行高性能转换
