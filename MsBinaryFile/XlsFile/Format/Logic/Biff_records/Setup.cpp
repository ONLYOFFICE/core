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

#include "Setup.h"
#include "../Biff_structures/BIFF12/XLWideString.h"
#include "../Biff_structures/BIFF12/RelID.h"

namespace XLS
{

Setup::Setup(bool isChart)
{
	numHdr.data.value = numFtr.data.value = 0.7875;
}
Setup::~Setup()
{}
BaseObjectPtr Setup::clone()
{
	return BaseObjectPtr(new Setup(*this));
}

void Setup::readFields(CFRecord& record)
{
	_UINT16 flags;

    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16 iPaperSize_2b;
        _UINT16 iScale_2b;
        _INT16 iPageStart_2b;
        _UINT16 iFitWidth_2b;
        _UINT16 iFitHeight_2b;

        record >> iPaperSize_2b >> iScale_2b >> iPageStart_2b >> iFitWidth_2b >> iFitHeight_2b >> flags;

        fLeftToRight = GETBIT(flags, 0);
        fPortrait	= GETBIT(flags, 1);
        fNoPls		= GETBIT(flags, 2);
        fNoColor	= GETBIT(flags, 3);
        fDraft		= GETBIT(flags, 4);
        fNotes		= GETBIT(flags, 5);
        fNoOrient	= GETBIT(flags, 6);
        fUsePage	= GETBIT(flags, 7);
        fEndNotes	= GETBIT(flags, 9);
        iErrors		= GETBITS(flags, 10, 11);

		iPaperSize = iPaperSize_2b;
		iScale = iScale_2b;
		iPageStart = iPageStart_2b;
		iFitWidth = iFitWidth_2b;
		iFitHeight = iFitHeight_2b;

		if (record.getGlobalWorkbookInfo()->Version > 0x0200)
		{
			_UINT16 iRes_2b;
			_UINT16 iVRes_2b;
			_UINT16 iCopies_2b;
			
			record >> iRes_2b >> iVRes_2b >> numHdr >> numFtr >> iCopies_2b;

			iRes = iRes_2b;
			iVRes = iVRes_2b;
			iCopies = iCopies_2b;
		}
    }
    else
    {
        if(_isChart)
        {
            _INT16 iPageStart_2b;
            record >> iPaperSize >> iRes >> iVRes >> iCopies >> iPageStart_2b >> flags;

            iPageStart = iPageStart_2b;

            fLandscape	= GETBIT(flags, 0);
            fNoColor	= GETBIT(flags, 2);
            fNoOrient	= GETBIT(flags, 3);
            fUsePage	= GETBIT(flags, 4);
            fDraft  	= GETBIT(flags, 5);

            XLSB::RelID str;
            record >> str;
            szRelID = str.value.value();
        }
        else
        {
            record >> iPaperSize >> iScale >> iRes >> iVRes >> iCopies >> iPageStart >> iFitWidth >> iFitHeight >> flags;

            fLeftToRight = GETBIT(flags, 0);
            fLandscape	= GETBIT(flags, 1);
            fNoColor	= GETBIT(flags, 3);
            fDraft		= GETBIT(flags, 4);
            fNotes		= GETBIT(flags, 5);
            fNoOrient	= GETBIT(flags, 6);
            fUsePage	= GETBIT(flags, 7);
            fEndNotes	= GETBIT(flags, 8);
            iErrors		= GETBITS(flags, 9, 10);

            XLSB::XLNullableWideString str;
            record >> str;
            szRelID = str.value();
        }
    }
}

void Setup::writeFields(CFRecord& record)
{
    _UINT16 flags = 0;

	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16 iPaperSize_2b = iPaperSize;
		_UINT16 iScale_2b = iScale;
		_INT16 iPageStart_2b = iPageStart;
		_UINT16 iFitWidth_2b = iFitWidth;
		_UINT16 iFitHeight_2b = iFitHeight;

		record << iPaperSize_2b << iScale_2b << iPageStart_2b << iFitWidth_2b << iFitHeight_2b;

		SETBIT(flags, 0, fLeftToRight)
		SETBIT(flags, 1, fPortrait)
		SETBIT(flags, 2, fNoPls)
		SETBIT(flags, 3, fNoColor)
		SETBIT(flags, 4, fDraft)
		SETBIT(flags, 5, fNotes)
		SETBIT(flags, 6, fNoOrient)
		SETBIT(flags, 7, fUsePage)
		SETBIT(flags, 9, fEndNotes)
		SETBITS(flags, 10, 11, iErrors)

		record << flags;

		if (record.getGlobalWorkbookInfo()->Version > 0x0200)
		{
			_UINT16 iRes_2b = iRes;
			_UINT16 iVRes_2b = iVRes;
			_UINT16 iCopies_2b = iCopies;

			record << iRes_2b << iVRes_2b << numHdr << numFtr << iCopies_2b;
		}
	}
	else
	{
		if (_isChart)
		{
			_INT16 iPageStart_2b = iPageStart;
			record << iPaperSize << iRes << iVRes << iCopies << iPageStart_2b;

			SETBIT(flags, 0, fLandscape);
			SETBIT(flags, 2, fNoColor);
			SETBIT(flags, 3, fNoOrient);
			SETBIT(flags, 4, fUsePage);
			SETBIT(flags, 5, fDraft);

			record << flags;

			XLSB::RelID str = szRelID;
			record << str;
		}
		else
		{
			record << iPaperSize << iScale << iRes << iVRes << iCopies << iPageStart << iFitWidth << iFitHeight;

			SETBIT(flags, 0, fLeftToRight)
			SETBIT(flags, 1, fLandscape)
			SETBIT(flags, 3, fNoColor)
			SETBIT(flags, 4, fDraft)
			SETBIT(flags, 5, fNotes)
			SETBIT(flags, 6, fNoOrient)
			SETBIT(flags, 7, fUsePage)
			SETBIT(flags, 8, fEndNotes)
			SETBITS(flags, 9, 10, iErrors)

			record << flags;

			XLSB::XLNullableWideString str = szRelID;
			record << str;
		}
	}
}

} // namespace XLS

