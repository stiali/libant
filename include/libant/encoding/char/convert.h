#ifndef LIBANT_INCLUDE_LIBANT_ENCODING_CHAR_CONVERT_H_
#define LIBANT_INCLUDE_LIBANT_ENCODING_CHAR_CONVERT_H_

#include <string>

namespace ant {

// UTF8 <--> GBK
std::string Utf8ToGbk(const std::string& input);
std::string Utf8ToGbk(const void* input, size_t inputLen);
std::string GbkToUtf8(const std::string& input);
std::string GbkToUtf8(const void* input, size_t inputLen);

// UTF8 <--> Unicode
#ifdef _WIN32
std::wstring Utf8ToUnicode(const std::string& input);
std::wstring Utf8ToUnicode(const void* input, int inputLen);
std::string UnicodeToUtf8(const std::wstring& input);
std::string UnicodeToUtf8(const wchar_t* input, int inputLen);
#endif

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_ENCODING_CHAR_CONVERT_H_
