#include <libant/include/libant/utils/string_utils.h>

using namespace std;

namespace ant {

vector<string> SplitString(const char* src, const char* sep, size_t sepLen)
{
    vector<string> result;

    const char* target = sep;
    const char* prePos = src;
    const char* curPos = src;
    while ((curPos = strstr(prePos, target)) != nullptr) {
        result.emplace_back(string(prePos, curPos));
        prePos = curPos + sepLen;
    }
    result.emplace_back(prePos);

    return result;
}

vector<string> SplitString(const string& src, char sep)
{
    vector<string> result;

    string::size_type prePos = 0;
    string::size_type curPos = src.find_first_of(sep);
    while (curPos != string::npos) {
        result.emplace_back(string(src.data() + prePos, curPos - prePos));
        prePos = curPos + 1;
        curPos = src.find_first_of(sep, prePos);
    }
    result.emplace_back(string(src, prePos));

    return result;
}

} // namespace ant