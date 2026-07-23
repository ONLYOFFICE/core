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

#include "SrvFmtCV.h"

using namespace XLS;

namespace XLSB
{

    SrvFmtCV::SrvFmtCV()
    {
    }

    SrvFmtCV::SrvFmtCV(XLS::CFRecord& record)
    {
        load(record);
    }

    SrvFmtCV::~SrvFmtCV()
    {
    }

    BiffStructurePtr SrvFmtCV::clone()
    {
        return BiffStructurePtr(new SrvFmtCV(*this));
    }

    void SrvFmtCV::load(XLS::CFRecord& record)
    {
        record >> ciRed >> ciGreen >> ciBlue;
        record.skipNunBytes(1); // unused
    }

	void SrvFmtCV::save(XLS::CFRecord& record)
	{
		record << ciRed << ciGreen << ciBlue;
		record.reserveNunBytes(1); // unused
	}

    _UINT32 SrvFmtCV::toHex()
    {
        return ((ciRed & 0xff) << 16) + ((ciGreen & 0xff) << 8) + (ciBlue & 0xff);
    }

	void SrvFmtCV::fromHex(_UINT32 hexValue)
	{
		ciRed = ((hexValue >> 16) & 0xFF) / 255.0;  // Extract the RR byte
		ciGreen = ((hexValue >> 8) & 0xFF) / 255.0;   // Extract the GG byte
		ciBlue = ((hexValue) & 0xFF) / 255.0;        // Extract the BB byte
	}

} // namespace XLSB

