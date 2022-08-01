#include "sector.h"

using namespace CFCPP;

int Sector::MINISECTOR_SIZE = 64;

Sector::Sector(int size, const Stream stream) :
    size(size), stream(stream)
{}

Sector::Sector(int size, const std::vector<BYTE>& data) :
    size(size), data(data)
{}

Sector::Sector(int size) :
    size(size)
{}


bool Sector::IsStreamed()
{
    if (stream == nullptr || size == MINISECTOR_SIZE)
        return false;

    auto currentPossition = stream->TellFile();

    stream->SeekFile(0, SEEK_END);
    auto endPossition = stream->TellFile();
    stream->SeekFile(0, SEEK_SET);
    auto begPossition = stream->TellFile();
    auto streamSize = endPossition - begPossition;

    stream->SeekFile(currentPossition, SEEK_SET);

    return  (this->id * size) + size < streamSize;
}

void Sector::ZeroData()
{
    std::fill(data.begin(), data.end(), 0);
    dirtyFlag = true;
}

void Sector::InitFATData()
{
    std::fill(data.begin(), data.end(), 0xff);
    dirtyFlag = true;
}

void Sector::ReleaseData()
{
    data.clear();
}

void Sector::Dispose(bool disposing)
{
    try
    {
        if (!_disposed)
        {
            std::lock_guard<std::mutex> lock(lockObject);
            if (disposing)
            {
                // Call from user code...


            }

            data.clear();
            dirtyFlag = false;
            id = ENDOFCHAIN;
            size = 0;

        }
    }
    catch(...)
    {}
    _disposed = true;
}

std::vector<BYTE> &Sector::GetData()
{
    if (data.empty())
    {
        data = std::vector<BYTE>(size, 0);
        if (IsStreamed())
        {
            DWORD bytesWasRead(0);
            stream->SeekFile(size + id * size, SEEK_SET);
            stream->ReadFile(data.data(), size, bytesWasRead);
        }
    }

    return data;
}

int Sector::getSize() const
{
    return size;
}
