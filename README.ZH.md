# 痛苦的 C++ 字符串转换

| [English](./README.md) | [中文](./README.ZH.md) |

为什么 C++ string/wstring/u8string/u16string/u32string 之间的转换这么痛苦？

## 用法

你可以通过以下 API 转换字符串：

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

你可以自定义命名空间和函数名称：

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

## 互转表格

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

* :white_check_mark: 可以转换。
* :warning: 始终假设 std::string 以 UTF-8 编码储存文本。
* Windows 的 std::wstring 可以看作 std::u16string。

# 待办事项

* [ ] 支持原始字符数组作为参数
* [ ] 支持 span 或者类似 span 的类型作为参数
* [ ] 清理代码减少冗余重复代码
* [ ] 支持使用 simdutf 库执行高性能转换

# C++ 标准库的字符串转换为什么这么痛苦？

## `char` 类型以及它的衍生物

C/C++ 语言中的字符和字符串主要由 `char` 字符类型，以及其组成的字符数组或字符串类型表示，所有需要和文本打交道的地方都能看到它的身影。

但是它存在大量的问题：

* `char` 是平台相关的，甚至会受到编译器编译选项的影响，它可能是 8 位有符号整数，也可能是 8 位无符号整数，甚至可以不是 8 位整数（比如一些嵌入式环境），具体位数可以通过 `CHAR_BIT` 宏检测（通常为 8）
* C/C++ 标准库中各种与 `char`/`char *`/`std::string` 等相关的 API 会受到区域设置影响，导致不同区域/代码页的设置会让代码执行的结果不一样，更糟糕的是标准 C/C++ 的区域设置并非线程安全，以至于 Windows 等平台都会开小灶提供非标准的带有区域设置参数版本的 API（比如 [`int _printf_l(const char* format, _locale_t locale, ...)`](https://learn.microsoft.com/en-us/cpp/c-runtime-library/reference/printf-printf-l-wprintf-wprintf-l?view=msvc-170)）
* `char` 及其组成的字符串经历了漫长的区域编码混乱时期，等 Unicode 标准推出、UTF-8 编码开始流行时，各种平台/操作系统、代码库、应用软件已经积重难返

## C

C 标准库提供了一系列区域（locale）、宽字符（wide-char）、多字节字符（multi-byte-char）相关的 API：

* `<stdlib.h>`
    * `mblen`
    * `mbtowc`
    * `wctomb`/`wctomb_s`
    * `mbstowcs`/`mbstowcs_s`
    * `wcstombs`/`wcstombs_s`
* `<wchar.h>`
    * `mbsinit`
    * `btowc`
    * `wctob`
    * `mbrlen`
    * `mbrtowc`
    * `wcrtomb`/`wcrtomb_s`
    * `mbsrtowcs`/`mbsrtowcs_s`
    * `wcsrtombs`/`wcsrtombs_s`

传统的多字节字符集受到区域设置的影响，可能会使用各种各样的区域编码，比如中国大陆使用 GB 码（GB2312、GBK、GB18030）、中国台湾使用 Big5、日本使用SHIFT-JIS 等，也可能使用 UTF-8 编码。

C 标准还提供了宽字符类型 `wchar_t`，用于表示“本机编码（native encoding）”。但这种类型是“实现定义的”，并没有一个公认的标准。Windows 平台规定 `wchar_t` 是 16 位字符类型，而它组成的字符串类型以 `UTF16-LE` 编码文本内容。Linux 平台通常规定 `wchar_t` 是 32位字符类型，但以如何编码文本内容并没有明确说明。而 Solaris/FreeBSD 平台的 `wchar_t` 甚至是和区域相关的，而且缺少文档。

[GNU官方文档表示 `wchar_t` 纯粹是毫无意义的垃圾，不应该使用。](https://www.gnu.org/software//gnulib/manual/html_node/The-wchar_005ft-type.html)

> Unfortunately, this API and its implementation has numerous problems:
>
> * On Windows platforms and on AIX in 32-bit mode, `wchar_t` is a 16-bit type. This means that it can never accommodate an entire Unicode character. Either the `wchar_t *` strings are limited to characters in UCS-2 (the “Basic Multilingual Plane” of Unicode), or – if `wchar_t *` strings are encoded in UTF-16 – a `wchar_t` represents only half of a character in the worst case, making the `<wctype.h>` functions pointless.
> * On Solaris and FreeBSD, the `wchar_t` encoding is locale dependent and undocumented. This means, if you want to know any property of a `wchar_t` character, other than the properties defined by `<wctype.h>` – such as whether it’s a dash, currency symbol, paragraph separator, or similar –, you have to convert it to `char *` encoding first, by use of the function `wctomb`.
> * When you read a stream of wide characters, through the functions `fgetwc` and `fgetws`, and when the input stream/file is not in the expected encoding, you have no way to determine the invalid byte sequence and do some corrective action. If you use these functions, your program becomes “garbage in - more garbage out” or “garbage in - abort”.
>
> As a consequence, it is better to use multibyte strings. Such multibyte strings can bypass limitations of the `wchar_t` type, if you use functions defined in Gnulib and GNU libunistring for text processing. They can also faithfully transport malformed characters that were present in the input, without requiring the program to **produce garbage** or abort.

> 遗憾的是，此 API 及其实现存在许多问题：
>
> * 在 Windows 平台和 32 位模式的 AIX 上，`wchar_t` 是一种16位类型。这意味着它永远无法容纳整个 Unicode 字符。要么 `wchar_t *` 字符串仅限于UCS-2（Unicode 的“基本多语言平面”）中的字符，要么——如果 `wchar_t *` 字符串以 UTF-16 编码——在最坏的情况下，`wchar_t` 只代表字符的一半，使得 `<wctype.h>` 函数毫无意义。
> * 在 Solaris 和 FreeBSD 上，`wchar_t` 编码依赖于区域设置，并且没有文档说明。这意味着，如果你想知道 `wchar_t` 字符的任何属性——例如它是破折号、货币符号、段落分隔符还是类似的属性——而不是 `<wctype.h>` 定义的属性，你必须首先使用函数 `wctomb` 将其转换为 `char *` 编码。
> * 当您通过函数 `fgetwc` 和 `fgetws` 读取宽字符流时，并且当输入流/文件不是预期的编码时，您无法确定无效的字节序列并采取一些纠正措施。如果你使用这些函数，你的程序就会变成“垃圾输入-更多垃圾输出”或“垃圾输入——崩溃”。
>
> 因此，最好使用多字节字符串。如果您使用 Gnulib 和 GNU libunistring 中定义的函数进行文本处理，则此类多字节字符串可以绕过 `wchar_t` 类型的限制。它们还可以忠实地传输输入中存在的格式错误的字符，而不需要程序**产生垃圾**或崩溃”。

从 C11 开始标准库中新增了 `char16_t` 和 `char32_t`；从 C23 开始 `char8_t` 也加入了标准库。以下是配套的 API：

* `<uchar.h>`
    * `mbrtoc8`
    * `c8rtomb`
    * `mbrtoc16`
    * `c16rtomb`
    * `mbrtoc32`
    * `c32rtomb`

有没有发现什么问题？在 C 语言中，传统的 `char` 类型始终作为一等公民。即使加入了 `char8_t`、`char16_t` 和 `char32_t`，这三种类型之间的互相转换仍然使用 `char` 作为中间媒介，而不能直接互相转换！这种设计让人感到匪夷所思。

而 C 标准对这些 API 的解释耐人寻味，比如 `c32rtomb`：

> Converts a single code point from its variable-length 32-bit wide character representation (but typically, UTF-32) to its narrow multibyte character representation.
>
> ...
>
> If the macro `__STDC_UTF_32__` is defined, the 32-bit encoding used by this function is UTF-32; otherwise, it is implementation-defined. In any case, the multibyte character encoding used by this function is specified by the currently active C locale.
>
> (since C23) The macro is always defined and the encoding is always UTF-32.

> 将单个码点从其可变长度 32 位宽字符表示形式（但通常为 UTF-32）转换为其窄多字节字符表示形式。
>
> ...
>
> 如果定义了宏 `__STDC_UTF_32__`，则此函数使用的 32 位编码为 UTF-32；否则，它是实现定义的。在任何情况下，此函数使用的多字节字符编码都是由当前活动的C语言环境指定的。
>
> （从 C23 开始）宏始终被定义，编码始终为 UTF-32。

C 标准委员会一开始的思路竟然是仅把 `char32_t` 当作一个 32 位可变长度字符类型，并不关心编码是什么，也不关心开发者会如何使用它，直到 C23 才后知后觉地强制它的编码为 UTF-32，填补了这个天坑。

C 标准委员会提供了 Unicode 字符串字面量（`u8""`、`u""`、`U""`），`char8_t`、`char16_t` 和 `char32_t` 类型，以及这些类型的字符和字符串转换到传统 `char` 多字节字符串的 API，然后就没有任何下文了。除了 `<uchar.h>` 中提供的那几个少得可怜的 API，C23 的标准库函数中再也见不到 `char8_t`、`char16_t` 和 `char32_t` 的影子，输入输出库、字符串操作库、区域库等依然活在 `char` 的世界里，任由开发者和平台/操作系统发挥主观能动性。这种做法似乎意味着 C 标准委员会希望开发者最多只在源文件中正确地储存 Unicode 编码的文本，并在运行时可以单向地转换到传统 `char` 多字节字符串，至于更多丰富的功能，烦请各位开发者出门左转用 icu4c 吧😅。

## C++

C++ 是 C 的超集，上一节提到的所有问题 C++ 都完整地继承了下来。

在 C 的基础上，C++ 还加入了以下内容用于字符和字符串转换：

* `<locale>`
    * `std::wstring_convert` (C++11) (deprecated in C++17) (removed in C++26)
    * `std::wbuffer_convert` (C++11) (deprecated in C++17) (removed in C++26)
    * `std::codecvt<char16_t, char, std::mbstate_t>` (since C++11) (deprecated in C++20)
    * `std::codecvt<char16_t, char8_t, std::mbstate_t>` (since C++11) (deprecated)
    * `std::codecvt<char32_t, char, std::mbstate_t>` (since C++11) (deprecated in C++20)
    * `std::codecvt<char32_t, char8_t, std::mbstate_t>` (since C++11) (deprecated)
* `<codecvt>`
    * `std::codecvt_utf8` (C++11) (deprecated in C++17) (removed in C++26)
    * `std::codecvt_utf16` (C++11) (deprecated in C++17) (removed in C++26)
    * `std::codecvt_utf8_utf16` (C++11) (deprecated in C++17) (removed in C++26)

尴尬的是这些类全都被弃用（deprecated）了，甚至还出现了 `char8_t` 相关的 `std::codecvt` 实现刚进标准库就立即宣布弃用的神经操作。被弃用后，接下来就是被移除（removed），可以看到在 C++26 标准中上面列出的类大部分都会被移除。

有得必有失，C++ 标准委员既然胆敢在 C++26 移除这些类，想必会有更好用的工具加入标准库吧？

遗憾的是，截至 2025 年 1 月，C++26 议程中并没有出现任何相关的提案，反而拉了坨大的：

* `<text_encoding>` Text encoding identifications (since C++26)
    * `text_encoding` (C++26) describes an interface for accessing the IANA Character Sets registry

> The class `text_encoding` provides a mechanism for identifying character encodings. It is used to determine the ordinary character literal encoding of the translation environment at compile-time and the character encoding of the execution environment at runtime.

> `text_encoding` 类提供了一种识别字符编码的机制。它用于确定编译时翻译环境的普通字符文字编码和运行时执行环境的字符编码。

当其他新兴编程语言基本上一致同意使用 Unicode 编码、用 UTF-8 作为源文件的编码格式时，C++ 标准委员会不打算进一步推动统一，反而继续强化各种混乱的区域编码概念，让开发者手动检测和处理所谓的字符串字面量编码、执行环境编码，简直是在开历史倒车。

## 总结

C/C++ 有着悠久的历史、沉重的历史包袱、大量的技术负债、分裂的编译器实现、草班台子标准委员会，种种原因导致 C/C++ 在文本编码问题上一片混沌，而且在未来依然会如此。
