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

#ifndef LIBANT_ENCODING_BINARY_BINARY_READER_H_
#define LIBANT_ENCODING_BINARY_BINARY_READER_H_

#include <string>
#include <string_view>
#include <libant/utils/os.h>

#include "endian.h"

namespace ant {

/**
 * Decode data from a binary stream.
 *
 * @tparam endian endian of the binary stream to be decoded
 */
template<Endian endian>
class BinaryReader {
public:
    /**
     * Construct a BinaryReader object from `stream`.
     *
     * @param stream
     */
    BinaryReader(std::string_view& stream)
        : buf_(stream)
        , pos_(0)
    {
    }

    /**
     * Construct a BinaryReader object from `data` of length `dataLen`.
     *
     * @param stream
     */
    BinaryReader(const void* data, size_t dataLen)
        : buf_(reinterpret_cast<const char*>(data), dataLen)
        , pos_(0)
    {
    }

    /**
     * Reads an integer from the binary stream into `val`.
     *
     * @tparam T supported integer types are int8, uint8, int16, uint16, int32, uint32, int64, and uint64
     * @param val
     *
     * @return true on success, false on failure.
     */
    template<typename T>
    bool ReadInteger(T& val)
    {
        auto finalPos = pos_ + sizeof(T);
        if (finalPos <= buf_.size()) {
            if constexpr (endian == Endian::LittleEndian) {
#ifdef LIBANT_NO_UNALIGNED_ACCESS
                memcpy(&val, &buf_[pos_], sizeof val);
                val = LittleEndianToHost(val);
#else
                val = LittleEndianToHost(*reinterpret_cast<const T*>(&buf_[pos_]));
#endif
            } else {
#ifdef LIBANT_NO_UNALIGNED_ACCESS
                memcpy(&val, &buf_[pos_], sizeof val);
                val = BigEndianToHost(val);
#else
                val = BigEndianToHost(*reinterpret_cast<const T*>(&buf_[pos_]));
#endif
            }
            pos_ = finalPos;
            return true;
        }
        return false;
    }

    /**
     * Reads the remaining bytes of the binary stream into `val`.
     *
     * @param val
     */
    void ReadString(std::string& val)
    {
        val.clear();
        val.append(&buf_[pos_], buf_.size() - pos_);
        pos_ = buf_.size();
    }

    /**
     * Reads length of the string of type `StringLenType` first, then reads the string payload according to the length into `val`.
     *
     * @tparam StringLenType type of the length of the string encoded before the string payload
     * @param val
     *
     * @return true on success, false on failure.
     */
    template<typename StringLenType>
    bool ReadString(std::string& val)
    {
        StringLenType len;
        if (ReadInteger(len) && len >= 0) {
            auto finalPos = pos_ + len;
            if (finalPos <= buf_.size()) {
                val.clear();
                val.append(&buf_[pos_], len);
                pos_ = finalPos;
                return true;
            }
        }
        return false;
    }

    /**
     * RemainingData returns a pointer to the unread data.
     *
     * @return pointer to the unread data.
     */
    const void* RemainingData() const
    {
        if (buf_.size() > pos_) {
            return buf_.data() + pos_;
        }
        return nullptr;
    }

    /**
     * RemainingLength returns length of the unread data.
     *
     * @return length of the unread data.
     */
    size_t RemainingLength() const
    {
        return buf_.size() - pos_;
    }

    /**
     * CurrentPosition returns the current reading position to the underlying buffer.
     *
     * @return current reading position to the underlying buffer.
     */
    size_t CurrentPosition() const
    {
        return pos_;
    }

private:
    std::string_view buf_;
    std::string_view::size_type pos_;
};

} // namespace ant

#endif //LIBANT_ENCODING_BINARY_BINARY_READER_H_
