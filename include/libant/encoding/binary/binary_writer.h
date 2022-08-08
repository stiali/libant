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

#ifndef LIBANT_ENCODING_BINARY_BINARY_WRITER_H_
#define LIBANT_ENCODING_BINARY_BINARY_WRITER_H_

#include <string>
#include <libant/utils/os.h>

#include "endian.h"

namespace ant {

/**
 * Encode data into a binary stream using the specified `endian`.
 *
 * @tparam endian endian used to encode data into the binary stream
 */
template<Endian endian>
class BinaryWriter {
public:
    using size_type = std::string::size_type;

public:
    /**
     * Construct a BinaryWriter object using `stream`.
     *
     * @param stream
     */
    BinaryWriter(std::string& stream)
        : buf_(stream)
    {
    }

    /**
     * Writes an integer into the binary stream.
     *
     * @tparam T supported integer types are int8, uint8, int16, uint16, int32, uint32, int64, and uint64
     * @param val
     */
    template<typename T>
    void WriteInteger(T val)
    {
        auto pos = buf_.size();
        buf_.resize(pos + sizeof val);
        if constexpr (endian == Endian::LittleEndian) {
#ifdef LIBANT_NO_UNALIGNED_ACCESS
            val = ant::HostToLittleEndian(val);
            memcpy(&buf_[pos], &val, sizeof val);
#else
            *reinterpret_cast<T*>(&buf_[pos]) = ant::HostToLittleEndian(val);
#endif
        } else {
#ifdef LIBANT_NO_UNALIGNED_ACCESS
            val = ant::HostToBigEndian(val);
            memcpy(&buf_[pos], &val, sizeof val);
#else
            *reinterpret_cast<T*>(&buf_[pos]) = ant::HostToBigEndian(val);
#endif
        }
    }

    /**
     * Writes a string into the binary stream.
     *
     * @param val
     */
    void WriteString(const std::string& val)
    {
        buf_.append(val);
    }

    /**
     * Writes string `s` of length `len` into the binary stream.
     *
     * @param s
     * @param len
     */
    void WriteString(const void* s, size_t len)
    {
        buf_.append(reinterpret_cast<const char*>(s), len);
    }

    /**
     * Writes length of the string of type `StringLenType` first, then writes the string payload into the binary stream.
     *
     * @tparam StringLenType type of the length of the string encoded before the string payload
     * @param val
     */
    template<typename StringLenType>
    void WriteString(const std::string& val)
    {
        WriteInteger(static_cast<StringLenType>(val.size()));
        WriteString(val);
    }

    /**
     * Appends n bytes of random data to the underlying string.
     */
    void Append(size_t n)
    {
        buf_.resize(buf_.size() + n);
    }

    /**
     * Return current size in bytes of the underlying string.
     */
    size_type Size() const
    {
        return buf_.size();
    }

private:
    std::string& buf_;
};

} // namespace ant

#endif //LIBANT_ENCODING_BINARY_BINARY_WRITER_H_
