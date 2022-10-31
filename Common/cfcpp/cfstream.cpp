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
#include "cfstream.h"
#include "cfexception.h"
#include "idirectoryentry.h"
#include "compoundfile_impl.h"
#include "Stream/stream_utils.h"

using namespace CFCPP;

CFStream::CFStream(CompoundFile_impl* compFile, std::weak_ptr<IDirectoryEntry> dirEntry) :
    CFItem(compFile)
{
    if (dirEntry.expired() || dirEntry.lock()->getSid() < 0)
        throw CFException("Attempting to create a CFStorage without DirectoryEntry");

    this->dirEntry = dirEntry;
}

void CFStream::SetData(const std::vector<BYTE> &data)
{
    CheckDisposed();

    compoundFile->FreeData(this);
    compoundFile->WriteData(shared_from_this(), data);
}

void CFStream::Write(const std::vector<BYTE> &data, _INT64 position)
{
    Write(data, position, 0, data.size());
}

void CFStream::Write(const std::vector<BYTE> &data, _INT64 position, _INT32 offset, _INT32 count)
{
    CheckDisposed();
    compoundFile->WriteData(shared_from_this(), data, position, offset, count);
}

void CFStream::Write(const char *data, _INT64 position, _INT32 count)
{
    CheckDisposed();
    compoundFile->WriteData(shared_from_this(), data, position, count);
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

_INT32 CFStream::Read(std::vector<BYTE> &buffer, _INT64 position, _INT32 count)
{
    CheckDisposed();
    return compoundFile->ReadData(this, position, buffer, 0, count);
}

_INT32 CFStream::Read(std::vector<BYTE> &buffer, _INT64 position, _INT32 offset, _INT32 count)
{
    CheckDisposed();
    return compoundFile->ReadData(this, position, buffer, offset, count);
}

void CFStream::CopyFrom(const Stream &input)
{
    CheckDisposed();

    std::vector<BYTE> buffer(Length(input));

//    if (CanSeek? )
    {
        input->seek(0, std::ios::beg);
    }

    input->read(reinterpret_cast<char*>(buffer.data()), Length(input));
    SetData(buffer);
}

void CFStream::Resize(_INT64 length)
{
    compoundFile->SetStreamLength(shared_from_this(), length);
}
