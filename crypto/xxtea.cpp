#include "xxtea.h"

namespace ant {

#define DELTA 0x9e3779b9
#define MX (((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) ^ ((sum ^ y) + (key_[(p & 3) ^ e] ^ z))

void XXTea::encrypt(uint32_t* data, uint32_t len)
{
    uint32_t n = len - 1;
    if (n >= 1) {
        uint32_t z = data[n], y, p, q = 6 + 52 / (n + 1), sum = 0, e;
        while (0 < q--) {
            sum += DELTA;
            e = sum >> 2 & 3;

            for (p = 0; p < n; p++) {
                y = data[p + 1];
                z = data[p] += MX;
            }

            y = data[0];
            z = data[n] += MX;
        }
    }
}

void XXTea::decrypt(uint32_t* data, uint32_t len)
{
    uint32_t n = len - 1;
    if (n >= 1) {
        uint32_t z, y = data[0], p, q = 6 + 52 / (n + 1), sum = q * DELTA, e;
        while (sum != 0) {
            e = sum >> 2 & 3;

            for (p = n; p > 0; p--) {
                z = data[p - 1];
                y = data[p] -= MX;
            }

            z = data[n];
            y = data[0] -= MX;
            sum -= DELTA;
        }
    }
}

} // namespace ant