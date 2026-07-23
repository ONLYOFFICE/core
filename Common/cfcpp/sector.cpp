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
#include "sector.h"
#include "Stream/stream_utils.h"


using namespace CFCPP;

_INT32 Sector::MINISECTOR_SIZE = 64;

Sector::Sector(_INT32 size, const Stream stream) :
    size(size),
    stream(stream)
{}

Sector::Sector(_INT32 size, const std::vector<BYTE>& data) :
    size(size), data(data)
{}

Sector::Sector(_INT32 size) :
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

_INT32 Sector::getSize() const
{
    return size;
}
