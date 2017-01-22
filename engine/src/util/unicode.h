#ifndef UNICODE_H
#define UNICODE_H

#include <string>

std::u32string unicode(const std::string& str);
std::string utf8(const std::u32string& str);

#endif /* UNICODE_H */

