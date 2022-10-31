#include "fstream_utils.h"
#include "fstream_wrapper.h"
#include "stream_utils.h"
#include "../../DesktopEditor/common/File.h"


using namespace CFCPP;

CFCPP::Stream CFCPP::OpenFileStream(std::wstring filename, bool bRewrite, bool trunc)
{
    BYTE* pUtf8 = nullptr;
    LONG lLen = 0;
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

_INT64 CFCPP::FileLenght(std::wstring filename)
{
    auto stream = OpenFileStream(filename);
    auto lenght = Length(stream);
    stream->close();

    return lenght;
}

ULONG64 CFCPP::FileFNVHash(std::wstring filename, _INT64 len, _INT64 offset)
{

    auto stream = OpenFileStream(filename);
    if (!IsOpen(stream))
        return 0;

    if (len < 0)
        len = Length(stream);

    stream->seek(offset);

    ULONG64 h = 2166136261;
    constexpr _INT64 bufLen = 0x2000;
    char buffer[bufLen];
    while (len > 0)
    {
        memset(buffer, 0, bufLen);
        _INT64 readLen = std::min(bufLen, len);
        stream->read(buffer, readLen);
        _INT64 i;

        for (i = 0; i < readLen; i++)
            h = (h * 16777619) ^ buffer[i];

        len -= readLen;
    }

    return h;
}
