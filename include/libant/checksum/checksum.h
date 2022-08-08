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

#ifndef LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
#define LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_

#include <cstddef>
#include <cstdint>

namespace ant {

/**
 * Add checksum of `data` to `currentChecksum`.
 *
 * @param currentChecksum
 * @param data
 * @param dataLen
 * @return checksum of `data` plus `currentChecksum`
 */
uint64_t AddChecksum(uint64_t currentChecksum, const void* data, size_t dataLen);

/**
 * fold the 64-bit checksum into a 32-bit checksum
 *
 * @param currentChecksum
 * @return the folded 32-bit checksum
 */
inline uint32_t FinishChecksum32(uint64_t currentChecksum)
{
    // fold the 64-bit checksum into a 32-bit checksum
    while (currentChecksum > 0xFFFFFFFF) {
        currentChecksum = (currentChecksum >> 32) + (currentChecksum & 0xFFFFFFFF);
    }
    return static_cast<uint32_t>(currentChecksum);
}

/**
 * fold the 64-bit checksum into a 16-bit checksum
 *
 * @param currentChecksum
 * @return the folded 16-bit checksum
 */
inline uint16_t FinishChecksum16(uint64_t currentChecksum)
{
    // fold the 64-bit checksum into a 16-bit checksum
    while (currentChecksum > 0xFFFFFFFF) {
        currentChecksum = (currentChecksum >> 32) + (currentChecksum & 0xFFFFFFFF);
    }
    while (currentChecksum > 0xFFFF) {
        currentChecksum = (currentChecksum >> 16) + (currentChecksum & 0xFFFF);
    }
    return static_cast<uint16_t>(currentChecksum);
}

/**
 * Checksum32 computes a 32-bit checksum of `data`
 *
 * @param data
 * @param dataLen
 *
 * @return a 32-bit checksum of `data`
 */
inline uint32_t Checksum32(const void* data, size_t dataLen)
{
    return FinishChecksum32(AddChecksum(0, data, dataLen));
}

/**
 * Checksum16 computes a 16-bit checksum of `data`
 *
 * @param data
 * @param dataLen
 *
 * @return a 16-bit checksum of `data`
 */
inline uint16_t Checksum16(const void* data, size_t dataLen)
{
    return FinishChecksum16(AddChecksum(0, data, dataLen));
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
