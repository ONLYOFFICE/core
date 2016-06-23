/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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

#include "PtgNameX.h"
#include "RevName.h"
#include <Binary/CFRecord.h>

namespace XLS
{


PtgNameX::PtgNameX()
{
}


PtgNameX::PtgNameX(const std::wstring& word, const PtgDataType data_type)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5))
{
	fromString(word);
}


BiffStructurePtr PtgNameX::clone()
{
	return BiffStructurePtr(new PtgNameX(*this));
}


void PtgNameX::storeFields(CFRecord& record)
{
	record << ixti << nameindex;
}


void PtgNameX::loadFields(CFRecord& record)
{
	record >> ixti;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		record.skipNunBytes(8);
		
		_UINT16 val;
		record >> val;

		nameindex = val;
		record.skipNunBytes(12);
	}
	else
		record >> nameindex;
	
	global_info = record.getGlobalWorkbookInfo();
}


void PtgNameX::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	RevNamePtr tab_id;
	if(!extra_data.empty() && (tab_id = boost::dynamic_pointer_cast<RevName>(extra_data.front())))
	{
#pragma message("####################### PtgNameX struct for revisions is not implemented")
		Log::error("PtgNameX struct for revisions is not implemented.");
		ptg_stack.push(L"#REF!");
		extra_data.pop();
		return;
	}

	std::wstring _Name;
	if(nameindex > 0 && nameindex <= global_info->AddinUdfs.size() && !(_Name = global_info->AddinUdfs[nameindex - 1]).empty())
	{
		ptg_stack.push(_Name);
	}
	else if(ixti > 0 && ixti <= global_info->xti_parsed.size())
	{
		std::wstring sheet = global_info->xti_parsed[ixti-1];

		if (!sheet.empty()) sheet += L"!";
		
		if (nameindex > 0 && nameindex <= global_info->arDefineNames.size())
		{
			_Name = global_info->arDefineNames[nameindex - 1];
		}

		if (sheet.empty() && _Name.empty() && nameindex <= global_info->arExternalNames.size() && nameindex > 0)
		{
			_Name = global_info->arExternalNames[nameindex - 1];
		}

		ptg_stack.push(sheet + _Name);
	}
	else
	{
#pragma message("####################### PtgNameX struct is not implemented")
 		Log::warning("PtgNameX structure is not implemented.");
		//ptg_stack.push(L"#UNDEFINED_EXTERN_NAME(" + STR::int2wstr(nameindex) + L")!");
		ptg_stack.push(L""); // This would let us to continue without an error
	}


	// Example of result: "[1]!range"
	// in the formula window it looks like: "'D:\Projects\AVSWorksheetConverter\bin\InFiles\Blank2003_range.xls'!range"
}


void PtgNameX::fromString(const std::wstring& word)
{
 	std::wstring  query = L"root/WorkbookStreamObject/GlobalsSubstream/SUPBOOK";
}


} // namespace XLS

