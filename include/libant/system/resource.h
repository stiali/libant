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

#ifndef LIBANT_SYSTEM_RESOURCE_H_
#define LIBANT_SYSTEM_RESOURCE_H_

#include <cstdint>

namespace ant {

/**
 * Set maximum core file size.
 * @param sizeInGB
 * @return true on success, false on failure
 */
bool SetMaxCoreFileSize(uint64_t sizeInGB);

/**
 * Set maximum number of open file descriptors.
 * @param maxFileNum
 * @return true on success, false on failure
 */
bool SetMaxOpenFiles(uint64_t maxFileNum);

} // namespace ant

#endif // LIBANT_SYSTEM_RESOURCE_H_
