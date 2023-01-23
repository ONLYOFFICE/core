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

#include "BoundSheet8.h"
#include <boost/lexical_cast.hpp>


namespace XLS
{

BoundSheet8::BoundSheet8()
{
}


BoundSheet8::~BoundSheet8()
{
}


BaseObjectPtr BoundSheet8::clone()
{
	return BaseObjectPtr(new BoundSheet8(*this));
}

void BoundSheet8::readFields(CFRecord& record)
{
	unsigned short flags;
	record >> lbPlyPos >> flags;
	
	if (record.getGlobalWorkbookInfo()->Version < 0x0600)
	{
		ShortXLAnsiString stName;
		record >> stName;
		name_ = stName.value();
	}
	else
	{
		ShortXLUnicodeString stName;
		record >> stName;
		name_ = stName.value();
	}

	switch(GETBITS(flags, 0, 1))
	{
		case 0:
			hsState = std::wstring (L"visible");
			break;
		case 1:
			hsState = std::wstring (L"hidden");
			break;
		case 2:
			hsState = std::wstring (L"veryHidden");
			break;
	}
	if (name_.length() > 31)
	{
		name_ = name_.substr(0, 31);
		//cell_links_broken.xls
	}
	if (name_.empty())
	{//file(6).xls
		name_ = L"Sheet_" + boost::lexical_cast<std::wstring>(record.getGlobalWorkbookInfo()->current_sheet + 1);
	}

	GlobalWorkbookInfo::_sheet_info sheet_info;
	sheet_info.state = hsState;
	sheet_info.name = name_;
	record.getGlobalWorkbookInfo()->sheets_info.push_back(sheet_info);
	
	dt = GETBITS(flags, 8, 15);
}

} // namespace XLS

