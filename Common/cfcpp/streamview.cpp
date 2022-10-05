#include "streamview.h"
#include "cfexception.h"
#include <cmath>

using namespace CFCPP;

StreamView::StreamView(const SVector<Sector> &sectorChain, int sectorSize, Stream stream)
    : sectorSize(sectorSize), sectorChain(sectorChain), stream(stream)
{
    if (sectorChain == nullptr)
        throw CFException("Sector Chain cannot be null");

    if (sectorSize <= 0)
        throw CFException("Sector size must be greater than zero");
}

StreamView::StreamView(const SVector<Sector> &sectorChain, int sectorSize, std::streamsize length,
                       SList<Sector> &availableSectors, Stream stream, bool isFatStream) :
    StreamView(sectorChain, sectorSize, stream)
{
    this->isFatStream = isFatStream;
    adjustLength(length, availableSectors);

}

std::streamsize StreamView::tell()
{
    return position;
}

void StreamView::write(const char *buffer, std::streamsize count)
{
    int byteWritten = 0;
    int roundByteWritten = 0;
    int offset = 0;

    if ((position + count) > length)
        adjustLength((position + count));

    if (sectorChain.empty() == false)
    {
        int secOffset = (int)(position / (std::streamsize)sectorSize);
        int secShift = (int)(position % sectorSize);

        roundByteWritten = (int)std::min(sectorSize - (position % (std::streamsize)sectorSize), count);

        if (secOffset < (int)sectorChain.size())
        {
            char* dst = reinterpret_cast<char*>(sectorChain[secOffset]->GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst + secShift);

            sectorChain[secOffset]->dirtyFlag = true;
        }

        byteWritten += roundByteWritten;
        offset += roundByteWritten;
        secOffset++;

        while (byteWritten < (count - sectorSize))
        {
            roundByteWritten = sectorSize;
            char* dst = reinterpret_cast<char*>(sectorChain[secOffset]->GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst);

            sectorChain[secOffset]->dirtyFlag = true;

            byteWritten += roundByteWritten;
            offset += roundByteWritten;
            secOffset++;
        }

        roundByteWritten = count - byteWritten;

        if (roundByteWritten != 0)
        {

            char* dst = reinterpret_cast<char*>(sectorChain[secOffset]->GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst);

            sectorChain[secOffset]->dirtyFlag = true;
        }

        position += count;

    }
}

void StreamView::close()
{
    if (std::dynamic_pointer_cast<std::iostream>(stream) != nullptr)
        stream->close();
}

std::streamsize StreamView::read(char *buffer, std::streamsize len)
{
    int nRead = 0;
    int nToRead = 0;
    int offset = 0;
    if (sectorChain.empty() == false && sectorChain.size() > 0)
    {
        int secIndex = (int)(position / (std::streamsize)sectorSize);

        nToRead = std::min((int)sectorChain[0]->GetData().size() - ((int)position % sectorSize), (int)len);

        if (secIndex < (int)sectorChain.size())
        {
            char* src = reinterpret_cast<char*>(sectorChain[secIndex]->GetData().data() + (int)(position % sectorSize));
            char* dst = buffer + offset;
            std::copy(src, src + nToRead, dst);
        }

        nRead += nToRead;

        secIndex++;

        while (nRead < (len - sectorSize))
        {
            nToRead = sectorSize;
            char* src = reinterpret_cast<char*>(sectorChain[secIndex]->GetData().data());
            char* dst = buffer + offset + nToRead;
            std::copy(src, src + nToRead, dst);

            nRead += nToRead;
            secIndex++;
        }

        nToRead = len - nRead;

        if (nToRead != 0)
        {
            if (secIndex > (int)sectorChain.size()) throw CFCorruptedFileException("The file is probably corrupted.");
            char* src = reinterpret_cast<char*>(sectorChain[secIndex]->GetData().data());
            char* dst = buffer + offset + nRead;
            std::copy(src, src + nToRead, dst);

            nRead += nToRead;
        }

        position += nRead;

        return nRead;

    }
    else
        return 0;
}

std::streamsize StreamView::seek(std::streamsize offset, std::ios_base::seekdir mode)
{
    switch (mode)
    {
    case std::ios_base::beg:
        position = offset;
        break;

    case std::ios_base::cur:
        position += offset;
        break;

    case std::ios_base::end:
    default:
        position = length - offset;
    }

    adjustLength(position);

    return position;
}

void StreamView::SetLength(std::streamsize value)
{
    adjustLength(value);
}

int StreamView::ReadInt32()
{
    read(reinterpret_cast<char*>(&buf), 4);
    return buf;
}

void StreamView::WriteInt32(int val)
{
    buf = ((val & 0xFF) << 24) | ((val & 0x00FF) << 16) | ((val & 0x0000FF) << 8) | (val & 0x000000FF);
    write(reinterpret_cast<char*>(&buf), 4);
}

void StreamView::adjustLength(std::streamsize value)
{
    SList<Sector> q;
    adjustLength(value, q);
}

void StreamView::adjustLength(std::streamsize value, SList<Sector> &availableSectors)
{
    this->length = value;

    std::streamsize delta = value - ((std::streamsize)this->sectorChain.size() * (std::streamsize)sectorSize);

    if (delta > 0)
    {
        int nSec = (int)std::ceil(((double)delta / sectorSize));

        while (nSec > 0)
        {
            std::shared_ptr<Sector> t;

            if (availableSectors.empty() || availableSectors.size() == 0)
            {
                t.reset(new Sector(sectorSize, stream));

                if (sectorSize == Sector::MINISECTOR_SIZE)
                    t->type = SectorType::Mini;
            }
            else
            {
                t = availableSectors.dequeue();
            }

            if (isFatStream)
            {
                t->InitFATData();
            }
            sectorChain.push_back(t);
            nSec--;
        }
    }
}

std::streamsize StreamView::getPosition() const
{
    return position;
}

std::streamsize StreamView::getLength() const
{
    return length;
}

SVector<Sector> &StreamView::BaseSectorChain()
{
    return sectorChain;
}
