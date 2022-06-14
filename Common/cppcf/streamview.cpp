#include "streamview.h"
#include "cfexception.h"
#include <cmath>

using namespace CFCPP;

StreamView::StreamView(const std::vector<Sector> &sectorChain, int sectorSize, const Stream &stream)
    : sectorSize(sectorSize), sectorChain(sectorChain), stream(stream)
{
    //    if (sectorChain == null)
    //        throw new CFException("Sector Chain cannot be null");

    if (sectorSize <= 0)
        throw new CFException("Sector size must be greater than zero");
}

StreamView::StreamView(const std::vector<Sector> &sectorChain, int sectorSize, std::streamsize length,
                       std::queue<Sector> &availableSectors, const Stream &stream, bool isFatStream) :
    StreamView(sectorChain, sectorSize, stream)
{
    this->isFatStream = isFatStream;
    adjustLength(length, availableSectors);

}

std::ostream &StreamView::Write(const char *buffer, std::streamsize offset, std::streamsize count)
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
            char* dst = reinterpret_cast<char*>(sectorChain[secOffset].GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst + secShift);

            sectorChain[secOffset].dirtyFlag = true;
        }

        byteWritten += roundByteWritten;
        offset += roundByteWritten;
        secOffset++;

        // Central sectors
        while (byteWritten < (count - sectorSize))
        {
            roundByteWritten = sectorSize;
            char* dst = reinterpret_cast<char*>(sectorChain[secOffset].GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst);

            sectorChain[secOffset].dirtyFlag = true;

            byteWritten += roundByteWritten;
            offset += roundByteWritten;
            secOffset++;
        }

        // Last sector
        roundByteWritten = count - byteWritten;

        if (roundByteWritten != 0)
        {

            char* dst = reinterpret_cast<char*>(sectorChain[secOffset].GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst);

            sectorChain[secOffset].dirtyFlag = true;

            offset += roundByteWritten;
            byteWritten += roundByteWritten;
        }

        position += count;

    }
}

int StreamView::ReadInt32()
{
    std::iostream::read(reinterpret_cast<char*>(buf), 4);
    return ((buf & 0xFF) << 24) | ((buf & 0x00FF) << 16) | ((buf & 0x0000FF) << 8) | (buf & 0x000000FF);
}

void StreamView::adjustLength(std::streamsize value)
{
    std::queue<Sector> q;
    adjustLength(value, q);
}

void StreamView::adjustLength(std::streamsize value, std::queue<Sector> &availableSectors)
{
    this->length = value;

    std::streamsize delta = value - ((std::streamsize)this->sectorChain.size() * (std::streamsize)sectorSize);

    if (delta > 0)
    {
        // enlargment required

        int nSec = (int)std::ceil(((double)delta / sectorSize));

        while (nSec > 0)
        {
            NSCommon::nullable<Sector> t;

            if (availableSectors.empty() || availableSectors.size() == 0)
            {
                t = new Sector(sectorSize, stream);

                if (sectorSize == Sector::MINISECTOR_SIZE)
                    t->type = SectorType::Mini;
            }
            else
            {
                t = availableSectors.front();
                availableSectors.pop();
            }

            if (isFatStream)
            {
                t->InitFATData();
            }
            sectorChain.push_back(std::move(t.get()));
            nSec--;
        }
    }
}
