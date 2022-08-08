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

#ifndef LIBANT_INCLUDE_LIBANT_UTILS_NONCOPYABLE_H_
#define LIBANT_INCLUDE_LIBANT_UTILS_NONCOPYABLE_H_

namespace ant {

class Noncopyable {
protected:
    Noncopyable() = default;

    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
};

#define NONCOPYABLE(class_)         \
    class_(const class_&) = delete; \
    class_& operator=(const class_&) = delete

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_UTILS_NONCOPYABLE_H_
