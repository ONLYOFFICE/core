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

#include "Qsi.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/XLWideString.h"

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
        unsigned short  flags1, flags2;
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

} // namespace XLS

