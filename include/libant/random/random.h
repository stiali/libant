/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2021 Antigloss Huang (https://github.com/antigloss) All rights reserved.
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

#ifndef LIBANT_RANDOM_RANDOM_H_
#define LIBANT_RANDOM_RANDOM_H_

#include <string>

namespace ant {

constexpr char printableChars_[] = R"(0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~)";

/**
 * Generate a random string with the given `len` and `engine`
 *
 * @tparam RandomNumberEngine engine() should return an unsigned integer randomly
 * @param len length of the randomly generated string
 * @param engine random engine used to generate the string
 * @return randomly generated string
 */
template<typename RandomNumberEngine>
std::string RandomString(size_t len, RandomNumberEngine& engine)
{
    std::string s;
    s.resize(len);
    for (size_t i = 0; i != len; ++i) {
        s[i] = printableChars_[engine() % (sizeof(printableChars_) - 1)];
    }
    return s;
}

} // namespace ant

#endif //LIBANT_RANDOM_RANDOM_H_
