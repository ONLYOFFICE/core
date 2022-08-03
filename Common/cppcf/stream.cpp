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

CFCPP::Stream CFCPP::OpenFileStream(std::wstring filename, bool bRewrite)
{
    BYTE* pUtf8 = nullptr;
    std::streamsize lLen = 0;
    NSFile::CUtf8Converter::GetUtf8StringFromUnicode(filename.c_str(), filename.length(), pUtf8, lLen, false);
    std::string utf8filename(pUtf8, pUtf8 + lLen);
    delete [] pUtf8;

    return OpenFileStream(utf8filename, bRewrite);
}

CFCPP::Stream CFCPP::OpenFileStream(std::string filename, bool bRewrite)
{
    filename = CorrectUnixPath(filename);

    CFCPP::Stream st;
    if (bRewrite)
        st.reset(new FStreamWrapper(filename, std::ios::app | std::ios::out | std::ios::binary | std::ios::in));
    else
        st.reset(new FStreamWrapper(filename, std::ios::in | std::ios::binary));

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

