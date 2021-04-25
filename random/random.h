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
