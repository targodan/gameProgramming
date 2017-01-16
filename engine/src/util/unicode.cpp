#include <locale>
#include <codecvt>

#include "unicode.h"

std::u32string unicode(const std::string& str) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.from_bytes(str.c_str());
}

std::string utf8(const std::u32string& str) {
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> convert;
    return convert.to_bytes(str.c_str());
}
