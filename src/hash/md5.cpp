#include <fstream>
#include <memory>
#include <openssl/md5.h>
#include <openssl/evp.h>

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

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (unlikely(!ctx)) {
        return false;
    }
    unique_ptr<EVP_MD_CTX, void (*)(EVP_MD_CTX*)> ctxGuard(ctx, EVP_MD_CTX_free);

    if (unlikely(!EVP_DigestInit_ex(ctx, EVP_md5(), nullptr))) {
        return false;
    }

    constexpr streamsize bufSize = 256 * 1024;
    char buf[bufSize];
    for (;;) {
        fin.read(buf, bufSize);
        if (unlikely(!EVP_DigestUpdate(ctx, buf, fin.gcount()))) {
            return false;
        }
        if (fin.gcount() < bufSize) {
            break;
        }
    }

    unsigned int mdLen = MD5_DIGEST_LENGTH;
    unsigned char md[MD5_DIGEST_LENGTH];
    if (unlikely(!EVP_DigestFinal_ex(ctx, md, &mdLen))) {
        return false;
    }

    md5.resize(MD5_DIGEST_LENGTH * 2);
    ant::ToLowerHexString(md, MD5_DIGEST_LENGTH, md5.data());
    return true;
}

} // namespace ant