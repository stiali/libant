#ifndef LIBANT_BITS_BITS_H_
#define LIBANT_BITS_BITS_H_

#include <cassert>
#include <climits>
#include <cstddef>

namespace ant::bits {

static uint64_t bit64_table[64] = {
    1ull,       1ull << 1,  1ull << 2,  1ull << 3,  1ull << 4,  1ull << 5,  1ull << 6,  1ull << 7,  1ull << 8,  1ull << 9,  1ull << 10, 1ull << 11, 1ull << 12,
    1ull << 13, 1ull << 14, 1ull << 15, 1ull << 16, 1ull << 17, 1ull << 18, 1ull << 19, 1ull << 20, 1ull << 21, 1ull << 22, 1ull << 23, 1ull << 24, 1ull << 25,
    1ull << 26, 1ull << 27, 1ull << 28, 1ull << 29, 1ull << 30, 1ull << 31, 1ull << 32, 1ull << 33, 1ull << 34, 1ull << 35, 1ull << 36, 1ull << 37, 1ull << 38,
    1ull << 39, 1ull << 40, 1ull << 41, 1ull << 42, 1ull << 43, 1ull << 44, 1ull << 45, 1ull << 46, 1ull << 47, 1ull << 48, 1ull << 49, 1ull << 50, 1ull << 51,
    1ull << 52, 1ull << 53, 1ull << 54, 1ull << 55, 1ull << 56, 1ull << 57, 1ull << 58, 1ull << 59, 1ull << 60, 1ull << 61, 1ull << 62, 1ull << 63};

static uint64_t bit64_not_table[64] = {~(1ull),       ~(1ull << 1),  ~(1ull << 2),  ~(1ull << 3),  ~(1ull << 4),  ~(1ull << 5),  ~(1ull << 6),  ~(1ull << 7),
                                       ~(1ull << 8),  ~(1ull << 9),  ~(1ull << 10), ~(1ull << 11), ~(1ull << 12), ~(1ull << 13), ~(1ull << 14), ~(1ull << 15),
                                       ~(1ull << 16), ~(1ull << 17), ~(1ull << 18), ~(1ull << 19), ~(1ull << 20), ~(1ull << 21), ~(1ull << 22), ~(1ull << 23),
                                       ~(1ull << 24), ~(1ull << 25), ~(1ull << 26), ~(1ull << 27), ~(1ull << 28), ~(1ull << 29), ~(1ull << 30), ~(1ull << 31),
                                       ~(1ull << 32), ~(1ull << 33), ~(1ull << 34), ~(1ull << 35), ~(1ull << 36), ~(1ull << 37), ~(1ull << 38), ~(1ull << 39),
                                       ~(1ull << 40), ~(1ull << 41), ~(1ull << 42), ~(1ull << 43), ~(1ull << 44), ~(1ull << 45), ~(1ull << 46), ~(1ull << 47),
                                       ~(1ull << 48), ~(1ull << 49), ~(1ull << 50), ~(1ull << 51), ~(1ull << 52), ~(1ull << 53), ~(1ull << 54), ~(1ull << 55),
                                       ~(1ull << 56), ~(1ull << 57), ~(1ull << 58), ~(1ull << 59), ~(1ull << 60), ~(1ull << 61), ~(1ull << 62), ~(1ull << 63)};

/**
 * @brief Set the n-th bit of the array on.
 *
 * @param arr
 * @param pos start from 1
 */
template<typename T, size_t N>
void SetOn(T (&arr)[N], size_t pos)
{
    assert((pos > 0) && (pos <= sizeof(arr) * CHAR_BIT));
    pos -= 1;
    size_t i = pos / (sizeof(T) * CHAR_BIT);
    arr[i] |= bit64_table[pos % (sizeof(T) * CHAR_BIT)];
}

/**
 * @brief Set the n-th bit of the array off.
 *
 * @param arr
 * @param pos start from 1
 */
template<typename T, size_t N>
void SetOff(T (&arr)[N], size_t pos)
{
    assert((pos > 0) && (pos <= sizeof(arr) * CHAR_BIT));
    pos -= 1;
    size_t i = pos / (sizeof(T) * CHAR_BIT);
    arr[i] &= bit64_not_table[pos % (sizeof(T) * CHAR_BIT)];
}

/**
 * @brief Test if the n-th bit of the array is on or off.
 *
 * @param arr
 * @param pos start from 1
 * @return true if the n-th bit is on, false otherwise
 */
template<typename T, size_t N>
bool Test(T (&arr)[N], size_t pos)
{
    assert((pos > 0) && (pos <= sizeof(arr) * CHAR_BIT));
    pos -= 1;
    size_t i = pos / (sizeof(T) * CHAR_BIT);
    return (arr[i] & bit64_table[pos % (sizeof(T) * CHAR_BIT)]);
}

/**
 * @brief Set the n-th bit of `val` on.
 *
 * @param val
 * @param pos start from 1
 * @return val | 1 << (pos - 1)
 */
template<typename T>
T SetOn(T val, size_t pos)
{
    assert((pos > 0) && (pos <= sizeof(T) * CHAR_BIT));
    return (val | bit64_table[pos - 1]);
}

/**
 * @brief Set the n-th bit of `val` off.
 *
 * @param val
 * @param pos start from 1
 * @return val & ~(1 << (pos - 1))
 */
template<typename T>
T SetOff(T val, size_t pos)
{
    assert((pos > 0) && (pos <= sizeof(T) * CHAR_BIT));
    return (val & bit64_not_table[pos - 1]);
}

/**
 * @brief Test if the n-th bit of `val` is on or off.
 *
 * @param val
 * @param pos start from 1
 * @return true if the n-th bit is on, false otherwise
 */
template<typename T>
bool Test(T val, size_t pos)
{
    assert((pos > 0) && (pos <= sizeof(T) * CHAR_BIT));
    return (val & bit64_table[pos - 1]);
}

} // namespace ant::bits

#endif //LIBANT_BITS_BITS_H_
