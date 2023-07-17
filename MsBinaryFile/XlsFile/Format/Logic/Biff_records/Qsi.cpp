/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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

#include "Qsi.h"
#include "../Biff_structures/BIFF12/XLWideString.h"

namespace XLS
{

Qsi::Qsi()
{
}

Qsi::~Qsi()
{
}

BaseObjectPtr Qsi::clone()
{
	return BaseObjectPtr(new Qsi(*this));
}

void Qsi::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        _UINT16			flags1, flags2;
        _UINT32         reserved;
        XLUnicodeString rgchName;

        record >> flags1 >> itblAutoFmt >> flags2 >> reserved >> rgchName;

        fTitles			= GETBIT(flags1, 0);
        fRowNums		= GETBIT(flags1, 1);
        fDisableRefresh	= GETBIT(flags1, 2);
        fAsync			= GETBIT(flags1, 3);
        fNewAsync		= GETBIT(flags1, 4);
        fAutoRefresh	= GETBIT(flags1, 5);
        fShrink			= GETBIT(flags1, 6);
        fFill			= GETBIT(flags1, 7);
        fAutoFormat		= GETBIT(flags1, 8);
        fSaveData		= GETBIT(flags1, 9);
        fDisableEdit	= GETBIT(flags1, 10);
        fOverwrite		= GETBIT(flags1, 13);

        fibitAtrNum		= GETBIT(flags2, 0);
        fibitAtrFnt		= GETBIT(flags2, 1);
        fibitAtrAlc		= GETBIT(flags2, 2);
        fibitAtrBdr		= GETBIT(flags2, 3);
        fibitAtrPat		= GETBIT(flags2, 4);
        fibitAtrProt	= GETBIT(flags2, 5);

        name            = rgchName.value();

        record.skipNunBytes(2);	//unused
    }

    else
    {
        _UINT32            flags;
        XLSB::XLWideString irstName;
        record >> flags >> itblAutoFmt >> dwConnID >> irstName;

        fTitles			= GETBIT(flags, 0);
        fRowNums		= GETBIT(flags, 1);
        fDisableRefresh	= GETBIT(flags, 2);
        fAsync			= GETBIT(flags, 3);
        fNewAsync		= GETBIT(flags, 4);
        fAutoRefresh	= GETBIT(flags, 5);
        fShrink			= GETBIT(flags, 6);

        fOverwrite		= GETBIT(flags, 7);
        fFill			= GETBIT(flags, 8);
        fSaveData		= GETBIT(flags, 9);
        fDisableEdit	= GETBIT(flags, 10);
        fPreserveFmt	= GETBIT(flags, 11);
        fAutoFit        = GETBIT(flags, 12);
        fDummyList      = GETBIT(flags, 13);

        fibitAtrNum		= GETBIT(flags, 14);
        fibitAtrFnt		= GETBIT(flags, 15);
        fibitAtrAlc		= GETBIT(flags, 16);
        fibitAtrBdr		= GETBIT(flags, 17);
        fibitAtrPat		= GETBIT(flags, 18);
        fibitAtrProt	= GETBIT(flags, 19);

        name            = irstName.value();
    }
}

void Qsi::writeFields(CFRecord& record)
{
	if (record.getGlobalWorkbookInfo()->Version < 0x0800)
	{
		_UINT16			flags1 = 0, flags2 = 0;
		_UINT32         reserved = 0;
		XLUnicodeString rgchName;

		SETBIT(flags1, 0, fTitles)
		SETBIT(flags1, 1, fRowNums)
		SETBIT(flags1, 2, fDisableRefresh)
		SETBIT(flags1, 3, fAsync)
		SETBIT(flags1, 4, fNewAsync)
		SETBIT(flags1, 5, fAutoRefresh)
		SETBIT(flags1, 6, fShrink)
		SETBIT(flags1, 7, fFill)
		SETBIT(flags1, 8, fAutoFormat)
		SETBIT(flags1, 9, fSaveData)
		SETBIT(flags1, 10, fDisableEdit)
		SETBIT(flags1, 13, fOverwrite)

		SETBIT(flags2, 0, fibitAtrNum)
		SETBIT(flags2, 1, fibitAtrFnt)
		SETBIT(flags2, 2, fibitAtrAlc)
		SETBIT(flags2, 3, fibitAtrBdr)
		SETBIT(flags2, 4, fibitAtrPat)
		SETBIT(flags2, 5, fibitAtrProt)

		rgchName = name;
		record << flags1 << itblAutoFmt << flags2 << reserved << rgchName;

		record.reserveNunBytes(2);	//unused
	}

	else
	{
		_UINT32            flags = 0;
		XLSB::XLWideString irstName;

		SETBIT(flags, 0, fTitles)
		SETBIT(flags, 1, fRowNums)
		SETBIT(flags, 2, fDisableRefresh)
		SETBIT(flags, 3, fAsync)
		SETBIT(flags, 4, fNewAsync)
		SETBIT(flags, 5, fAutoRefresh)
		SETBIT(flags, 6, fShrink)

		SETBIT(flags, 7, fOverwrite)
		SETBIT(flags, 8, fFill)
		SETBIT(flags, 9, fSaveData)
		SETBIT(flags, 10, fDisableEdit)
		SETBIT(flags, 11, fPreserveFmt)
		SETBIT(flags, 12, fAutoFit)
		SETBIT(flags, 13, fDummyList)

		SETBIT(flags, 14, fibitAtrNum)
		SETBIT(flags, 15, fibitAtrFnt)
		SETBIT(flags, 16, fibitAtrAlc)
		SETBIT(flags, 17, fibitAtrBdr)
		SETBIT(flags, 18, fibitAtrPat)
		SETBIT(flags, 19, fibitAtrProt)

		irstName = name;

		record << flags << itblAutoFmt << dwConnID << irstName;
	}
}

} // namespace XLS

