#include <string>
#include <libant/buffer_pool/buffer_pool.h>

using namespace std;

int main()
{
    auto pool = ant::BufferPool<string, string::size_type, &string::capacity, &string::clear>::CreateBufferPool(100 * 1024 * 1024);
    auto buf = pool->GetBuffer();
    buf = pool->GetBuffer();
    buf = pool->GetBuffer();
    pool.reset();
}