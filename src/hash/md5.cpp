#include <fstream>
#include <openssl/md5.h>

#include <libant/encoding/hex/hex.h>
#include <libant/utils/likely.h>
#include <libant/hash/md5.h>

using namespace std;

namespace ant {

string MD5File(const string& filepath)
{
    string sum;

    ifstream fin(filepath, ifstream::binary);
    if (unlikely(!fin)) {
        return sum;
    }

    MD5_CTX ctx;
    if (unlikely(!MD5_Init(&ctx))) {
        return sum;
    }

    constexpr streamsize bufSize = 1024 * 1024;
    char buf[bufSize];
    for (auto n = fin.readsome(buf, bufSize); n > 0; n = fin.readsome(buf, bufSize)) {
        if (unlikely(!MD5_Update(&ctx, buf, n))) {
            return sum;
        }
        if (n < bufSize) {
            break;
        }
    }

    unsigned char md5[MD5_DIGEST_LENGTH];
    if (unlikely(!MD5_Final(md5, &ctx))) {
        return sum;
    }

    sum.resize(MD5_DIGEST_LENGTH * 2);
    ant::ToLowerHexString(md5, MD5_DIGEST_LENGTH, sum.data());
    return sum;
}

} // namespace ant