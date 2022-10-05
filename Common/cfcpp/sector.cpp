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

    auto fileSize = Length(stream);
    return  (this->id * size) + size < fileSize;
}

void Sector::ZeroData()
{
    std::fill(data.begin(), data.end(), 0);
    dirtyFlag = true;
}

void Sector::InitFATData()
{
    data.clear();
    data.resize(size);
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
            stream->seek(size + id * size, std::ios_base::beg);
            stream->read(reinterpret_cast<char*>(data.data()), size);
        }
    }

    return data;
}

int Sector::getSize() const
{
    return size;
}
