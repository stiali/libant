#include <fstream>
#include <openssl/md5.h>

#include <libant/encoding/hex/hex.h>
#include <libant/utils/likely.h>
#include <libant/hash/md5.h>

using namespace std;

namespace ant {

bool MD5File(const std::string& filepath, std::string& md5)
{
    ifstream fin(filepath, ifstream::binary);
    if (unlikely(!fin)) {
        return false;
    }

    MD5_CTX ctx;
    if (unlikely(!MD5_Init(&ctx))) {
        return false;
    }

    constexpr streamsize bufSize = 256 * 1024;
    char buf[bufSize];
    for (;;) {
        fin.read(buf, bufSize);
        if (unlikely(!MD5_Update(&ctx, buf, fin.gcount()))) {
            return false;
        }
        if (fin.gcount() < bufSize) {
            break;
        }
    }

    unsigned char md[MD5_DIGEST_LENGTH];
    if (unlikely(!MD5_Final(md, &ctx))) {
        return false;
    }

    md5.resize(MD5_DIGEST_LENGTH * 2);
    ant::ToLowerHexString(md, MD5_DIGEST_LENGTH, md5.data());
    return true;
}

} // namespace ant