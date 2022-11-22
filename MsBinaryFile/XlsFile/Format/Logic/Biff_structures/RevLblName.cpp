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

#include "RevLblName.h"
#include "../../Binary/CFRecord.h"
#include "../../../../../OOXML/XlsbFormat/Biff12_structures/XLWideString.h"

namespace XLS
{


RevLblName::RevLblName()
{
}


RevLblName::RevLblName(CFRecord& record)
{
	load(record);
}


BiffStructurePtr RevLblName::clone()
{
	return BiffStructurePtr(new RevLblName(*this));
}

void RevLblName::load(CFRecord& record)
{
    XLUnicodeString       st_raw;
    XLSB::XLWideString    st_raw_xlsb;
	unsigned char iBuiltin;
    record >> iBuiltin;

    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
    {
        record >> st_raw;
    }
    else
    {
        record >> st_raw_xlsb;
    }

	switch(iBuiltin)
	{
		case 0x00:        
            if (record.getGlobalWorkbookInfo()->Version < 0x0800)
            {
                st = st_raw.value();
            }
            else
            {
                st = st_raw_xlsb.value();
            }
			break;
		case 0x01:
			st = L"_xlnm.Consolidate_Area";
			break;
		case 0x02:
			st = L"_xlnm.Auto_Open";
			break;
		case 0x03:
			st = L"_xlnm.Auto_Close";
			break;
		case 0x04:
			st = L"_xlnm.Extract";
			break;
		case 0x05:
			st = L"_xlnm.Database";
			break;
		case 0x06:
			st = L"_xlnm.Criteria";
			break;
		case 0x07:
			st = L"_xlnm.Print_Area";
			break;
		case 0x08:
			st = L"_xlnm.Print_Titles";
			break;
		case 0x09:
			st = L"_xlnm.Recorder";
			break;
		case 0x0A:
			st = L"_xlnm.Data_Form";
			break;
		case 0x0B:
			st = L"_xlnm.Auto_Activate";
			break;
		case 0x0C:
			st = L"_xlnm.Auto_Deactivate";
			break;
		case 0x0D:
			st = L"_xlnm.Sheet_Title";
			break;
		case 0x0E:
			st = L"_xlnm._FilterDatabase";
			break;
		default:
			// EXCEPT::RT::WrongBiffRecord("Unsupported value of built-in name.", record.getTypeString());
			break;
	}
}


const std::wstring RevLblName::toString() const
{
	return st;
}


} // namespace XLS

