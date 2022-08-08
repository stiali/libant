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

#ifndef LIBANT_INCLUDE_LIBANT_UTILS_LIKELY_H_
#define LIBANT_INCLUDE_LIBANT_UTILS_LIKELY_H_

#ifndef _WIN32

#define likely(cond_) __builtin_expect(!!(cond_), 1)
#define unlikely(cond_) __builtin_expect(!!(cond_), 0)

#else

#define likely(cond_) cond_
#define unlikely(cond_) cond_

#endif

#endif //LIBANT_INCLUDE_LIBANT_UTILS_LIKELY_H_
