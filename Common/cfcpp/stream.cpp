#include "stream.h"
#include "../../DesktopEditor/common/File.h"
#include <algorithm>


std::streamsize CFCPP::Length(const CFCPP::Stream& st)
{
    if (st.get() == nullptr)
        return 0;

    auto curPos = st->tell();
    st->seek(0, std::ios_base::end);
    auto ssize = st->tell();
    st->seek(curPos);

    return ssize;
}

CFCPP::Stream CFCPP::OpenFileStream(std::wstring filename, bool bRewrite, bool trunc)
{
    BYTE* pUtf8 = nullptr;
    std::streamsize lLen = 0;
    NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename.c_str(), filename.length(), pUtf8, lLen, false);
    std::string utf8filename(pUtf8, pUtf8 + lLen);
    delete [] pUtf8;

    return OpenFileStream(utf8filename, bRewrite, trunc);
}

CFCPP::Stream CFCPP::OpenFileStream(std::string filename, bool bRewrite, bool trunc)
{
    CFCPP::Stream st;

    // it's not good, but otherwise file doesn't create or if use ios::app, then the seek for writing will be blocked
    if (bRewrite)
        std::fstream create(filename, std::ios::app | std::ios::out);

    if (trunc && bRewrite)
        st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in | std::ios::out | std::ios::trunc));
    else if (bRewrite)
        st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in | std::ios::out));
    else
        st.reset(new FStreamWrapper(filename, std::ios::binary | std::ios::in));

    return st;
}

bool CFCPP::IsOpen(const Stream &st)
{
    if (std::dynamic_pointer_cast<FStreamWrapper>(st))
        return std::static_pointer_cast<FStreamWrapper>(st)->is_open();

    return false;
}

std::string CFCPP::CorrectUnixPath(const std::string original)
{
    #if !defined(_WIN32) && !defined (_WIN64)
    return original;
    #else
        auto str = original;
        std::replace(str.begin(), str.end(), '/', '\\');
        return str;
    #endif
}


int CFCPP::FileLenght(std::wstring filename)
{
    auto stream = OpenFileStream(filename);
    auto lenght = Length(stream);
    stream->close();

    return lenght;
}

ULONG64 CFCPP::FileSimpleHash(std::wstring filename, int len, int offset)
{

    auto stream = OpenFileStream(filename);
    if (!IsOpen(stream))
        return 0;

    if (len < 0)
        len = Length(stream);

    stream->seek(offset);

    ULONG64 h = 2166136261;
    constexpr int bufLen = 0x2000;
    char buffer[bufLen];
    while (len > 0)
    {
        memset(buffer, 0, bufLen);
        int readLen = std::min(bufLen, len);
        stream->read(buffer, readLen);
        int i;

        for (i = 0; i < readLen; i++)
            h = (h * 16777619) ^ buffer[i];

        len -= readLen;
    }


    return h;
}
