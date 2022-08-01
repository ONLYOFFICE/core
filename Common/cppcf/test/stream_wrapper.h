#pragma once
#include "stream.h"


CFCPP::Stream getStream(std::wstring filename, bool bWriteble = false)
{
    CFCPP::Stream st(new NSFile::CStreamWriter);
    bool status;
    if (bWriteble)
        status = st->CreateFileW(filename);
    else
        status = st->OpenFile(filename);

    if (status)
        return st;
    else
        return CFCPP::Stream();
}

void flush(CFCPP::Stream& st)
{
    st->Flush();
}

void seek(CFCPP::Stream& st, int pos)
{
    st->SeekFile(pos, SEEK_SET);
}

long tell(CFCPP::Stream& st)
{
    return st->TellFile();
}

long size(CFCPP::Stream& st)
{
    return st->SizeFile();
}
