#pragma once

#include "stream.h"


namespace CFCPP
{
class StreamRW
{
public:
    StreamRW(const Stream &stream);
    T_LONG64 Seek(T_LONG64 offset);

    template <class T>
    T Read()
    {
        DWORD bytesWasRead(0);
        T value;
        auto asByteArr = reinterpret_cast<BYTE*>(&value);
        stream->ReadFile(asByteArr, sizeof (T), bytesWasRead);
        return value;
    }

    template<class T>
    void Write(T value)
    {
        auto asByteArr = reinterpret_cast<BYTE*>(&value);
        stream->WriteFile(asByteArr, sizeof (T));
    }

    inline void ReadArray(BYTE* data, DWORD lenght)
    {
        DWORD bytesWasRead;
        stream->ReadFile(data, lenght, bytesWasRead);
    }
    inline void WriteArray(const BYTE *data, DWORD lenght)
    {
        stream->WriteFile(data, lenght);
    }

    inline void Close()
    {
        stream->CloseFile();
    }

private:
    Stream stream;
};

}
