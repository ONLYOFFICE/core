/*
 * (c) Copyright Ascensio System SIA 2010-2019
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#include "streamview.h"
#include "cfexception.h"
#include <cmath>


using namespace CFCPP;

StreamView::StreamView(const SVector<Sector> &sectorChain, int sectorSize, Stream stream)
    : sectorSize(sectorSize),
      sectorChain(sectorChain),
      stream(stream)
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
        int sectorOffset = (int)(position / (std::streamsize)sectorSize);
        int sectorShift = (int)(position % sectorSize);

        roundByteWritten = (int)(std::min)(sectorSize - (position % (std::streamsize)sectorSize), count);

        if (sectorOffset < (int)sectorChain.size())
        {
            char* dst = reinterpret_cast<char*>(sectorChain[sectorOffset]->GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst + sectorShift);

            sectorChain[sectorOffset]->dirtyFlag = true;
        }

        byteWritten += roundByteWritten;
        offset += roundByteWritten;
        sectorOffset++;

        while (byteWritten < (count - sectorSize))
        {
            roundByteWritten = sectorSize;
            char* dst = reinterpret_cast<char*>(sectorChain[sectorOffset]->GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst);

            sectorChain[sectorOffset]->dirtyFlag = true;

            byteWritten += roundByteWritten;
            offset += roundByteWritten;
            sectorOffset++;
        }

        roundByteWritten = count - byteWritten;

        if (roundByteWritten != 0)
        {

            char* dst = reinterpret_cast<char*>(sectorChain[sectorOffset]->GetData().data());
            std::copy(buffer+offset, buffer+offset+roundByteWritten, dst);

            sectorChain[sectorOffset]->dirtyFlag = true;
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
        int sectorIndex = (int)(position / (std::streamsize)sectorSize);

        nToRead = (std::min)((int)sectorChain[0]->GetData().size() - ((int)position % sectorSize), (int)len);

        if (sectorIndex < (int)sectorChain.size())
        {
            char* src = reinterpret_cast<char*>(sectorChain[sectorIndex]->GetData().data() + (int)(position % sectorSize));
            char* dst = buffer + offset;
            std::copy(src, src + nToRead, dst);
        }

        nRead += nToRead;
        sectorIndex++;

        while (nRead < (len - sectorSize))
        {
            nToRead = sectorSize;
            char* src = reinterpret_cast<char*>(sectorChain[sectorIndex]->GetData().data());
            char* dst = buffer + offset + nToRead;
            std::copy(src, src + nToRead, dst);

            nRead += nToRead;
            sectorIndex++;
        }

        nToRead = len - nRead;

        if (nToRead != 0)
        {
            if (sectorIndex > (int)sectorChain.size()) throw CFCorruptedFileException("The file is probably corrupted.");
            char* src = reinterpret_cast<char*>(sectorChain[sectorIndex]->GetData().data());
            char* dst = buffer + offset + nRead;
            std::copy(src, src + nToRead, dst);

            nRead += nToRead;
        }

        position += nRead;

        return nRead;

    }

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
        int numberSector = (int)std::ceil(((double)delta / sectorSize));

        while (numberSector > 0)
        {
            std::shared_ptr<Sector> newSector;

            if (availableSectors.empty() || availableSectors.size() == 0)
            {
                newSector.reset(new Sector(sectorSize, stream));

                if (sectorSize == Sector::MINISECTOR_SIZE)
                    newSector->type = SectorType::Mini;
            }
            else
            {
                newSector = availableSectors.dequeue();
            }

            if (isFatStream)
            {
                newSector->InitFATData();
            }
            sectorChain.push_back(newSector);
            numberSector--;
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
