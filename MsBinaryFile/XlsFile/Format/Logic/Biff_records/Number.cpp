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
#include "Number.h"
#include "../../Auxiliary/HelpFunc.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

Number_BIFF2::Number_BIFF2()
{}
Number_BIFF2::~Number_BIFF2()
{}
BaseObjectPtr Number_BIFF2::clone()
{
	return BaseObjectPtr(new Number_BIFF2(*this));
}
//---------------------------------------------------------------------------------
Number::Number()
{}
Number::~Number()
{}
BaseObjectPtr Number::clone()
{
	return BaseObjectPtr(new Number(*this));
}
void Number::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	if (record.getDataSize() == 15)
	{
		//wrong version !! 
		int store = global_info_->Version;
		global_info_->Version = 0x0200;
		
		record >> cell >> num;

		global_info_->Version = store;
	}
	else
	{ // sizeof record == 14
		record >> cell >> num;
	}


	_INT32 val = 0;
	if (record.getDataSize() >= 18)//SchetPrintForm.xls
	{
		record >> val;
	}
}

void Number::writeFields(CFRecord& record)
{
    record << cell << num;
}

const CellRef Number::getLocation() const
{
	return cell.getLocation();
}
int Number::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)    
    {
		int row = cell.rw;
			
		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if (cell.ixfe >= global_info_->cellStyleXfs_count)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}
			CP_XML_NODE(L"v")
			{
				CP_XML_STREAM() << XmlUtils::ToString(num.data.value);
			}
		}			
	}
	return 0;
}
//---------------------------------------------------------------------------------
Integer_BIFF2::Integer_BIFF2() : num(0)
{}
Integer_BIFF2::~Integer_BIFF2()
{}
BaseObjectPtr Integer_BIFF2::clone()
{
	return BaseObjectPtr(new Integer_BIFF2(*this));
}
void Integer_BIFF2::readFields(CFRecord& record)
{//only version 0x0200 
	global_info_ = record.getGlobalWorkbookInfo();

	int store = global_info_->Version;
	global_info_->Version = 0x0200;

	record >> cell;

	if (record.getRdPtr() + 2 < record.getDataSize())
	{
		record >> num;
	}
	else
	{
		_INT16 num_2byte = 0;
		record >> num_2byte;
		num = num_2byte;
	}
	
	global_info_->Version = store;
}
const CellRef Integer_BIFF2::getLocation() const
{
	return cell.getLocation();
}
int Integer_BIFF2::serialize(std::wostream & stream)
{
	CP_XML_WRITER(stream)
	{
		int row = cell.rw;

		std::wstring ref = cell.getLocation().toString();// getColRowRef(i, row);
		CP_XML_NODE(L"c")
		{
			CP_XML_ATTR(L"r", ref);

			if (cell.ixfe >= global_info_->cellStyleXfs_count)
			{
				CP_XML_ATTR(L"s", cell.ixfe - global_info_->cellStyleXfs_count);
			}
			CP_XML_NODE(L"v")
			{
				CP_XML_STREAM() << std::to_wstring(num);
			}
		}
	}
	return 0;
}
} // namespace XLS

