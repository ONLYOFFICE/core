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

#include "Selection.h"

namespace XLS
{

Selection::Selection()
{
}


Selection::~Selection()
{
}


BaseObjectPtr Selection::clone()
{
	return BaseObjectPtr(new Selection(*this));
}

void Selection::readFields(CFRecord& record)
{
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        unsigned short cref;
        R_RwU			rwAct_2b;
        ColU			colAct_2b;
        _INT16			irefAct_2b;

        record >> pnn >> rwAct_2b >> colAct_2b >> irefAct_2b >> cref;

        rwAct   = rwAct_2b;
        colAct  = colAct_2b;
        irefAct = irefAct_2b;

        activeCell = static_cast<std::wstring >(CellRef(rwAct_2b, colAct_2b, true, true));
        std::wstring  sqref_str;
        for(int i = 0; i < cref; ++i)
        {
            RefU refu;
            record >> refu;
            sqref_str += std::wstring (refu.toString(false).c_str()) + ((i == cref - 1) ? L"" : L" ");
        }
        sqref = sqref_str;
    }
    else
    {
        XLSB::UncheckedSqRfX    sqrfx;
        record >> pnn_xlsb >> rwAct >> colAct >> irefAct >> sqrfx;
        activeCell = static_cast<std::wstring >(CellRef(rwAct, colAct, true, true));
        std::wstring  sqref_str;
        int i = 0, cref = sqrfx.rgrfx.size();

        std::for_each(sqrfx.rgrfx.begin(), sqrfx.rgrfx.end(), [&](XLSB::UncheckedRfX &refu) {
            sqref_str += std::wstring (refu.toString(false).c_str()) + ((i == cref - 1) ? L"" : L" ");
        });

        sqref = sqref_str;
    }
}

int Selection::serialize(std::wostream & stream)
{	
	if (pnn == (unsigned char)PaneType::REVTPNNTOPLEFT &&
		activeCell == L"A1"		&&
		irefAct == (_INT16)0		&&
		sqref == L"A1") return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"selection")
		{
			switch(pnn)
			{
			case PaneType::REVTPNNBOTRIGHT:	CP_XML_ATTR(L"pane", L"bottomRight");	break;
			case PaneType::REVTPNNTOPRIGHT:	CP_XML_ATTR(L"pane", L"topRight");		break;
			case PaneType::REVTPNNBOTLEFT:	CP_XML_ATTR(L"pane", L"bottomLeft");	break;
			//	default:					CP_XML_ATTR(L"pane", L"topLeft"); 
			}

			if (activeCell != L"A1") 
				CP_XML_ATTR(L"activeCell", activeCell);

			if (irefAct != 0) 
				CP_XML_ATTR(L"activeCellId", irefAct);

			if (sqref != L"A1") 
				CP_XML_ATTR(L"sqref", sqref);
		}
	}
	return 0;
}

} // namespace XLS

