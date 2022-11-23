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
#pragma once

#include <iostream>
#include <list>
#include "sector.h"
#include "slist.h"
#include "svector.h"

namespace CFCPP
{
class StreamView : public IStream
{
public:
    StreamView(const SVector<Sector> &sectorChain, _INT32 sectorSize, Stream stream);
    StreamView(const SVector<Sector> &sectorChain, _INT32 sectorSize, _INT64 length,
               SList<Sector> &availableSectors, Stream stream, bool isFatStream = false);


    _INT64 tell() override;
    _INT64 seek(_INT64 offset, std::ios_base::seekdir mode = std::ios::beg) override;
    _INT64 read(char *buffer, _INT64 count) override;
    void write(const char *buffer, _INT64 count) override;
    void flush() override {}
    void close() override;


    _INT64 getPosition() const;
    void SetLength(_INT64 value);
    _INT64 getLength() const;
    SVector<Sector>& BaseSectorChain();

    _INT32 ReadInt32();
    void WriteInt32(_INT32 val);

private:
    void adjustLength(_INT64 value);
    void adjustLength(_INT64 value, SList<Sector> &availableSectors);

private:
    _INT32 sectorSize = 0;
    _INT64 length = 0;

    SVector<Sector> sectorChain;
    bool isFatStream = false;
    _INT32 buf = 0;

    Stream stream;
    _INT64 position = 0;
    SList<Sector> freeSectors;
};
}
