#include "cfstream.h"
#include "cfexception.h"
#include "idirectoryentry.h"
#include "compoundfile.h"

using namespace CFCPP;

CFStream::CFStream(CompoundFile* compFile, std::weak_ptr<IDirectoryEntry> dirEntry) :
    CFItem(compFile)
{
    if (dirEntry.expired() || dirEntry.lock()->getSid() < 0)
        throw CFException("Attempting to create a CFStorage using an unitialized directory");

    this->dirEntry = dirEntry;
}

void CFStream::SetData(const std::vector<BYTE> &data)
{
    CheckDisposed();

    compoundFile->FreeData(this);
    compoundFile->WriteData(shared_from_this(), data);
}

void CFStream::Write(const std::vector<BYTE> &data, std::streamsize position)
{
    Write(data, position, 0, data.size());
}

void CFStream::Write(const std::vector<BYTE> &data, std::streamsize position, int offset, int count)
{
    CheckDisposed();
    compoundFile->WriteData(shared_from_this(), data, position, offset, count);
}

void CFStream::Append(const std::vector<BYTE> &data)
{
    CheckDisposed();
    if (size() > 0)
    {
        compoundFile->AppendData(shared_from_this(), data);
    }
    else
    {
        compoundFile->WriteData(shared_from_this(), data);
    }
}

std::vector<BYTE> CFStream::getData() const
{
    CheckDisposed();

    return compoundFile->GetData(this);
}

int CFStream::Read(std::vector<BYTE> &buffer, std::streamsize position, int count)
{
    CheckDisposed();
    return compoundFile->ReadData(this, position, buffer, 0, count);
}

int CFStream::Read(std::vector<BYTE> &buffer, std::streamsize position, int offset, int count)
{
    CheckDisposed();
    return compoundFile->ReadData(this, position, buffer, offset, count);
}

void CFStream::CopyFrom(const Stream &input)
{
    CheckDisposed();

    std::vector<BYTE> buffer(input->SizeFile());

//    if (input.CanSeek)
    {
        input->SeekFile(0, SEEK_SET);
    }

    DWORD bytesWasRead(0);
    input->ReadFile(buffer.data(), input->SizeFile(), bytesWasRead);
    SetData(buffer);
}

void CFStream::Resize(std::streamsize length)
{
    compoundFile->SetStreamLength(shared_from_this(), length);
}
