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
