/*
 * (c) Copyright Ascensio System SIA 2010-2021
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

#include "WsProp.h"
#include "../../../MsBinaryFile/XlsFile/Format/Logic/Biff_structures/BIFF12/CellRef.h"

using namespace XLS;

namespace XLSB
{

    WsProp::WsProp()
    {
    }

    WsProp::~WsProp()
    {
    }

    BaseObjectPtr WsProp::clone()
    {
        return BaseObjectPtr(new WsProp(*this));
    }

    void WsProp::readFields(XLS::CFRecord& record)
    {
        _UINT16 flags1;
        BYTE flags2;
        record >> flags1 >> flags2;

        fShowAutoBreaks          = GETBIT(flags1, 0);
        fPublish            	 = GETBIT(flags1, 3);
        fDialog                  = GETBIT(flags1, 4);
        fApplyStyles             = GETBIT(flags1, 5);
        fRowSumsBelow            = GETBIT(flags1, 6);
        fColSumsRight            = GETBIT(flags1, 7);
        fFitToPage               = GETBIT(flags1, 8);
        fShowOutlineSymbols      = GETBIT(flags1, 10);
        fSyncHoriz               = GETBIT(flags1, 12);
        fSyncVert                = GETBIT(flags1, 13);
        fAltExprEval             = GETBIT(flags1, 14);
        fAltFormulaEntry         = GETBIT(flags1, 15);

        fFilterMode              = GETBIT(flags2, 16);
        fCondFmtCalc             = GETBIT(flags2, 17);

        brtcolorTab.readFields(record);
        record >> rwSync >> colSync >> strName;
        if(rwSync != 0xFFFFFFFF && colSync != 0xFFFFFFFF)
            syncRef = static_cast<std::wstring >(CellRef(rwSync, colSync, true, true));
    }

	void WsProp::writeFields(XLS::CFRecord& record)
	{
		_UINT16 flags1 = 0;
		BYTE flags2 = 0;		

		SETBIT(flags1, 0, fShowAutoBreaks)
		SETBIT(flags1, 3, fPublish)
		SETBIT(flags1, 4, fDialog)
		SETBIT(flags1, 5, fApplyStyles)
		SETBIT(flags1, 6, fRowSumsBelow)
		SETBIT(flags1, 7, fColSumsRight)
		SETBIT(flags1, 8, fFitToPage)
		SETBIT(flags1, 10, fShowOutlineSymbols)
		SETBIT(flags1, 12, fSyncHoriz)
		SETBIT(flags1, 13, fSyncVert)
		SETBIT(flags1, 14, fAltExprEval)
		SETBIT(flags1, 15, fAltFormulaEntry)

		SETBIT(flags2, 16, fFilterMode)
		SETBIT(flags2, 17, fCondFmtCalc)

		record << flags1 << flags2;

		brtcolorTab.writeFields(record);

		if(syncRef.empty())
		{
			rwSync = 0xFFFFFFFF;
			colSync = 0xFFFFFFFF;
			record << rwSync << colSync;
		}
		else
		{
			CellRef ref(syncRef);
			rwSync = ref.getRow();
			colSync = ref.getColumn();
			record << rwSync << colSync;
		}

		record << strName;		
	}

} // namespace XLSB

