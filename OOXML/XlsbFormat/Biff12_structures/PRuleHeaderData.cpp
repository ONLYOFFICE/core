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

#include "PRuleHeaderData.h"

using namespace XLS;

namespace XLSB
{

    PRuleHeaderData::PRuleHeaderData()
    {
    }

    PRuleHeaderData::PRuleHeaderData(XLS::CFRecord& record)
    {
        load(record);
    }

    PRuleHeaderData::~PRuleHeaderData()
    {
    }

    BiffStructurePtr PRuleHeaderData::clone()
    {
        return BiffStructurePtr(new PRuleHeaderData(*this));
    }

    void PRuleHeaderData::load(XLS::CFRecord& record)
    {
        _UINT32 flags;
        record >> isxvd >> flags;

        isxrtype        = GETBITS(flags, 0, 7);
        fDataOnly       = GETBIT(flags, 8);
        fLabelOnly      = GETBIT(flags, 9);
        fGrandRw        = GETBIT(flags, 10);
        fGrandCol       = GETBIT(flags, 11);
        fCacheBased     = GETBIT(flags, 12);
        fLineMode       = GETBIT(flags, 13);
        fPart           = GETBIT(flags, 14);
        fFuzzy          = GETBIT(flags, 15);
        sxaxis.bRw      = GETBIT(flags, 16);
        sxaxis.bCol     = GETBIT(flags, 17);
        sxaxis.bPage    = GETBIT(flags, 18);
        sxaxis.bData    = GETBIT(flags, 19);
        iDim            = GETBITS(flags, 20, 27);

        if(fPart)
            record >> rfxLoc;
    }

	void PRuleHeaderData::save(XLS::CFRecord& record)
	{
		_UINT32 flags = 0;

		SETBITS(flags, 0, 7, isxrtype)
		SETBIT(flags, 8, fDataOnly)
		SETBIT(flags, 9, fLabelOnly)
		SETBIT(flags, 10, fGrandRw)
		SETBIT(flags, 11, fGrandCol)
		SETBIT(flags, 12, fCacheBased)
		SETBIT(flags, 13, fLineMode)
		SETBIT(flags, 14, fPart)
		SETBIT(flags, 15, fFuzzy)
		SETBIT(flags, 16, sxaxis.bRw)
		SETBIT(flags, 17, sxaxis.bCol)
		SETBIT(flags, 18, sxaxis.bPage)
		SETBIT(flags, 19, sxaxis.bData)
		SETBITS(flags, 20, 27, iDim)

		record << isxvd << flags;

		if (fPart)
			record << rfxLoc;
	}

} // namespace XLSB

