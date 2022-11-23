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

#include "Setup.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/XLWideString.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/RelID.h"

namespace XLS
{

Setup::Setup(bool isChart)
: _isChart(isChart), iPaperSize(0), iScale(255), iRes(0), iVRes(0), iCopies(0), fNoOrient(false), fPortrait(false), iPageStart(1), iErrors(0)
{
	numHdr.data.value = numFtr.data.value = 0.5;
}
Setup::~Setup()
{}
BaseObjectPtr Setup::clone()
{
	return BaseObjectPtr(new Setup(*this));
}

void Setup::readFields(CFRecord& record)
{
	unsigned short flags;

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

} // namespace XLS

