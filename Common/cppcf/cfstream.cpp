#include "cfstream.h"
#include "cfexception.h"

using namespace CFCPP;

CFStream::CFStream(std::weak_ptr<CompoundFile> compFile, std::shared_ptr<IDirectoryEntry> dirEntry) :
    CFItem(compFile)
{
    if (dirEntry == nullptr || dirEntry->getSid() < 0)
        throw new CFException("Attempting to create a CFStorage using an unitialized directory");

    this->dirEntry = dirEntry;
}

void CFStream::SetData(const std::vector<BYTE> &data)
{
    CheckDisposed();

    compoundFile.lock()->FreeData(this);
    compoundFile.lock()->WriteData(this, data);
}

void CFStream::Write(const std::vector<BYTE> &data, std::streamsize position)
{
    Write(data, position, 0, data.size());
}

void CFStream::Write(const std::vector<BYTE> &data, std::streamsize position, int offset, int count)
{
    CheckDisposed();
    compoundFile.lock()->WriteData(this, data, position, offset, count);
}

void CFStream::Append(const std::vector<BYTE> &data)
{
    CheckDisposed();
    if (size() > 0)
    {
        compoundFile.lock()->AppendData(this, data);
    }
    else
    {
        compoundFile.lock()->WriteData(this, data);
    }
}

std::vector<BYTE> CFStream::getData() const
{
    CheckDisposed();

    return compoundFile.lock()->GetData(this);
}

int CFStream::Read(std::vector<BYTE> &buffer, std::streamsize position, int count)
{
    CheckDisposed();
    return compoundFile.lock()->ReadData(this, position, buffer, 0, count);
}

int CFStream::Read(std::vector<BYTE> &buffer, std::streamsize position, int offset, int count)
{
    CheckDisposed();
    return compoundFile.lock()->ReadData(this, position, buffer, offset, count);
}

void CFStream::CopyFrom(const Stream &input)
{
    CheckDisposed();

    std::vector<BYTE> buffer(Length(input));

//    if (input.CanSeek)
    {
        input->seekg(0, std::ios::beg);
    }

    input->read(reinterpret_cast<char*>(buffer.data()), Length(input));
    SetData(buffer);
}

void CFStream::Resize(std::streamsize length)
{
    compoundFile.lock()->SetStreamLength(this, length);
}
