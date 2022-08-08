/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2022 Antigloss Huang (https://github.com/antigloss) All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

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
