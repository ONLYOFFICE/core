#include "streamrw.h"
#include <algorithm>

using namespace CFCPP;

StreamRW::StreamRW(const Stream &stream)
    : stream(stream)
{}

T_LONG64 StreamRW::Seek(T_LONG64 offset)
{
    if(!stream->SeekFile(offset, SEEK_SET))
        return -1;

    return stream->TellFile();
}
