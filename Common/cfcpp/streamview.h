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
	bool isError() override;

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
