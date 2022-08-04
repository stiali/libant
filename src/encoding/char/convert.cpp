#ifdef _WIN32
#include <Windows.h>
#endif

#include <libant/encoding/char/convert.h>

using namespace std;

namespace ant {

enum CharEncodingType {
#ifdef _WIN32
    kEncodingGBK = CP_ACP,
    kEncodingUTF8 = CP_UTF8,
#else
    // TODO andy:
    kEncodingGBK,
    kEncodingUTF8,
#endif
};

inline string convertEncoding(const char* input, int inputLen, CharEncodingType fromEncoding, CharEncodingType toEncoding)
{
#ifdef _WIN32
    int len = MultiByteToWideChar(fromEncoding, 0, input, inputLen, 0, 0);
    wstring wcharBuf(len, 0);
    MultiByteToWideChar(fromEncoding, 0, input, inputLen, &(wcharBuf[0]), len);
    int len2 = WideCharToMultiByte(toEncoding, 0, wcharBuf.data(), len, 0, 0, 0, 0);
    string charBuf(len2, 0);
    WideCharToMultiByte(toEncoding, 0, wcharBuf.data(), len, &(charBuf[0]), len2, 0, 0);
    return charBuf;
#else
    return "Not yet implemented!";
#endif
}

string Utf8ToGbk(const string& input)
{
    return convertEncoding(input.c_str(), static_cast<int>(input.size()), kEncodingUTF8, kEncodingGBK);
}

string Utf8ToGbk(const void* input, int inputLen)
{
    return convertEncoding(reinterpret_cast<const char*>(input), inputLen, kEncodingUTF8, kEncodingGBK);
}

string GbkToUtf8(const string& input)
{
    return convertEncoding(input.c_str(), static_cast<int>(input.size()), kEncodingGBK, kEncodingUTF8);
}

string GbkToUtf8(const void* input, int inputLen)
{
    return convertEncoding(reinterpret_cast<const char*>(input), inputLen, kEncodingGBK, kEncodingUTF8);
}

std::wstring Utf8ToUnicode(const std::string& input)
{
    return Utf8ToUnicode(input.c_str(), static_cast<int>(input.size()));
}

std::wstring Utf8ToUnicode(const void* input, int inputLen)
{
#ifdef _WIN32
    wstring wcharBuf;
    wcharBuf.resize(inputLen);
    auto len = MultiByteToWideChar(kEncodingUTF8, 0, reinterpret_cast<const char*>(input), inputLen, &(wcharBuf[0]), inputLen);
    wcharBuf.resize(len);
    return wcharBuf;
#else
    return wstring(); // TODO andy:
#endif
}

std::string UnicodeToUtf8(const std::wstring& input)
{
    return UnicodeToUtf8(input.c_str(), static_cast<int>(input.size()));
}

std::string UnicodeToUtf8(const wchar_t* input, int inputLen)
{
#ifdef _WIN32
    auto outLen = inputLen * 3;
    string charBuf;
    charBuf.resize(outLen);
    outLen = WideCharToMultiByte(kEncodingUTF8, 0, input, inputLen, &(charBuf[0]), outLen, 0, 0);
    charBuf.resize(outLen);
    return charBuf;
#else
    return string();  // TODO andy:
#endif
}

} // namespace ant