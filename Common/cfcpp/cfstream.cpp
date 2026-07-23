/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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
