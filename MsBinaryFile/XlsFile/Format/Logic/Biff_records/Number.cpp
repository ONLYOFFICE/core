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

	record >> cell >> num;

	_INT32 val = 0;
	if (record.getDataSize() >= 18)//SchetPrintForm.xls
	{
		record >> val;
	}
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
Integer_BIFF2::Integer_BIFF2()
{}
Integer_BIFF2::~Integer_BIFF2()
{}
BaseObjectPtr Integer_BIFF2::clone()
{
	return BaseObjectPtr(new Integer_BIFF2(*this));
}
void Integer_BIFF2::readFields(CFRecord& record)
{
	global_info_ = record.getGlobalWorkbookInfo();

	record >> cell >> num;
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

