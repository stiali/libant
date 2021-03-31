#include <cstring>
#include "chacha20.h"

namespace ant {

// The following code snippets are extracted from boringssl.

// sigma contains the ChaCha constants, which happen to be an ASCII string.
static const uint8_t sigma[16] = {'e', 'x', 'p', 'a', 'n', 'd', ' ', '3', '2', '-', 'b', 'y', 't', 'e', ' ', 'k'};

#define U32TO8_LITTLE(p, v)        \
    {                              \
        (p)[0] = (v >> 0) & 0xff;  \
        (p)[1] = (v >> 8) & 0xff;  \
        (p)[2] = (v >> 16) & 0xff; \
        (p)[3] = (v >> 24) & 0xff; \
    }

#define U8TO32_LITTLE(p) (((uint32_t)((p)[0])) | ((uint32_t)((p)[1]) << 8) | ((uint32_t)((p)[2]) << 16) | ((uint32_t)((p)[3]) << 24))

#define ROTATE(v, n) (((v) << (n)) | ((v) >> (32 - (n))))

// QUARTERROUND updates a, b, c, d with a ChaCha "quarter" round.
#define QUARTERROUND(a, b, c, d)    \
    x[a] += x[b];                   \
    x[d] = ROTATE(x[d] ^ x[a], 16); \
    x[c] += x[d];                   \
    x[b] = ROTATE(x[b] ^ x[c], 12); \
    x[a] += x[b];                   \
    x[d] = ROTATE(x[d] ^ x[a], 8);  \
    x[c] += x[d];                   \
    x[b] = ROTATE(x[b] ^ x[c], 7);

// chacha_core performs 20 rounds of ChaCha on the input words in
// |input| and writes the 64 output bytes to |output|.
static void chacha_core(uint8_t output[64], const uint32_t input[16])
{
    uint32_t x[16];
    int i;

    memcpy(x, input, sizeof(uint32_t) * 16);
    for (i = 20; i > 0; i -= 2) {
        QUARTERROUND(0, 4, 8, 12)
        QUARTERROUND(1, 5, 9, 13)
        QUARTERROUND(2, 6, 10, 14)
        QUARTERROUND(3, 7, 11, 15)
        QUARTERROUND(0, 5, 10, 15)
        QUARTERROUND(1, 6, 11, 12)
        QUARTERROUND(2, 7, 8, 13)
        QUARTERROUND(3, 4, 9, 14)
    }

    for (i = 0; i < 16; ++i) {
        x[i] += input[i];
    }
    for (i = 0; i < 16; ++i) {
        U32TO8_LITTLE(output + 4 * i, x[i]);
    }
}

//=========================================================================
// ChaCha20Cipher Public Methods
//=========================================================================

ChaCha20Cipher::ChaCha20Cipher(const uint8_t key[32], const uint8_t nonce[12], uint32_t counter)
    : counter_(counter)
{
    input_[0] = U8TO32_LITTLE(sigma + 0);
    input_[1] = U8TO32_LITTLE(sigma + 4);
    input_[2] = U8TO32_LITTLE(sigma + 8);
    input_[3] = U8TO32_LITTLE(sigma + 12);

    input_[4] = U8TO32_LITTLE(key + 0);
    input_[5] = U8TO32_LITTLE(key + 4);
    input_[6] = U8TO32_LITTLE(key + 8);
    input_[7] = U8TO32_LITTLE(key + 12);

    input_[8] = U8TO32_LITTLE(key + 16);
    input_[9] = U8TO32_LITTLE(key + 20);
    input_[10] = U8TO32_LITTLE(key + 24);
    input_[11] = U8TO32_LITTLE(key + 28);

    input_[13] = U8TO32_LITTLE(nonce + 0);
    input_[14] = U8TO32_LITTLE(nonce + 4);
    input_[15] = U8TO32_LITTLE(nonce + 8);
}

void ChaCha20Cipher::Encrypt(void* dst, const void* src, size_t len)
{
    input_[12] = counter_;

    auto out = reinterpret_cast<uint8_t*>(dst);
    auto in = reinterpret_cast<const uint8_t*>(src);
    uint8_t buf[64];
    size_t todo, i;

    while (len > 0) {
        todo = sizeof(buf);
        if (len < todo) {
            todo = len;
        }

        chacha_core(buf, input_);
        for (i = 0; i < todo; i++) {
            out[i] = in[i] ^ buf[i];
        }

        out += todo;
        in += todo;
        len -= todo;

        ++input_[12];
    }
}

} // namespace ant