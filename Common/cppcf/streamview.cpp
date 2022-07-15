#include "streamview.h"
#include "cfexception.h"
#include <cmath>

using namespace CFCPP;

StreamView::StreamView(const SVector<Sector> &sectorChain, int sectorSize, Stream stream)
    : sectorSize(sectorSize), sectorChain(sectorChain), stream(stream)
{
    //    if (sectorChain == null)
    //        throw new CFException("Sector Chain cannot be null");

    if (sectorSize <= 0)
        throw new CFException("Sector size must be greater than zero");
}

StreamView::StreamView(const SVector<Sector> &sectorChain, int sectorSize, std::streamsize length,
                       SList<Sector> &availableSectors, Stream stream, bool isFatStream) :
    StreamView(sectorChain, sectorSize, stream)
{
    this->isFatStream = isFatStream;
    adjustLength(length, availableSectors);

}

void StreamView::Write(const char *buffer, std::streamsize offset, std::streamsize count)
{
    int byteWritten = 0;
    int roundByteWritten = 0;

    // Assure length
    if ((position + count) > length)
        adjustLength((position + count));

    if (sectorChain.empty() == false)
    {
        // First sector
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

        // Central sectors
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

        // Last sector
        roundByteWritten = count - byteWritten;

        if (roundByteWritten != 0)
        {

            char* dst = reinterpret_cast<char*>(sectorChain[secOffset]->GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst);

            sectorChain[secOffset]->dirtyFlag = true;

            offset += roundByteWritten;
            byteWritten += roundByteWritten;
        }

        position += count;

    }
}

std::streamsize StreamView::Read(char *buffer, std::streamsize offset, std::streamsize count)
{
    int nRead = 0;
    int nToRead = 0;
    if (sectorChain.empty() == false && sectorChain.size() > 0)
    {
        // First sector
        int secIndex = (int)(position / (std::streamsize)sectorSize);

        // Bytes to read count is the min between request count
        // and sector border

        nToRead = std::min((int)sectorChain[0]->GetData().size() - ((int)position % sectorSize), (int)count);

        if (secIndex < (int)sectorChain.size())
        {
            char* src = reinterpret_cast<char*>(sectorChain[secIndex]->GetData().data() + (int)(position % sectorSize));
            char* dst = buffer + offset;
            std::copy(src, src + nToRead, dst);
        }

        nRead += nToRead;

        secIndex++;

        // Central sectors
        while (nRead < (count - sectorSize))
        {
            nToRead = sectorSize;
            char* src = reinterpret_cast<char*>(sectorChain[secIndex]->GetData().data());
            char* dst = buffer + offset + nToRead;
            std::copy(src, src + nToRead, dst);

            nRead += nToRead;
            secIndex++;
        }

        // Last sector
        nToRead = count - nRead;

        if (nToRead != 0)
        {
            if (secIndex > (int)sectorChain.size()) throw new CFCorruptedFileException("The file is probably corrupted.");
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

std::streamsize StreamView::Seek(std::streamsize offset, int origin)
{
    switch (origin)
    {
    case std::ios_base::beg:
        position = offset;
        break;

    case std::ios_base::cur:
        position += offset;
        break;

    case std::ios_base::end:
        position = length - offset;
        break;
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
    std::iostream::read(reinterpret_cast<char*>(&buf), 4);
    return ((buf & 0xFF) << 24) | ((buf & 0x00FF) << 16) | ((buf & 0x0000FF) << 8) | (buf & 0x000000FF);
}

void StreamView::WriteInt32(int val)
{
    buf = ((val & 0xFF) << 24) | ((val & 0x00FF) << 16) | ((val & 0x0000FF) << 8) | (val & 0x000000FF);
    std::iostream::write(reinterpret_cast<char*>(&buf), 4);
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
        // enlargment required

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

std::streamsize StreamView::getLength() const
{
    return length;
}
