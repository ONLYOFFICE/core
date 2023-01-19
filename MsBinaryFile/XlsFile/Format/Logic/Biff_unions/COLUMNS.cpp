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

#include "COLUMNS.h"
#include "../Biff_records/DefColWidth.h"
#include "../Biff_records/ColInfo.h"

namespace XLS
{


COLUMNS::COLUMNS()
{
}


COLUMNS::~COLUMNS()
{
}


BaseObjectPtr COLUMNS::clone()
{
	return BaseObjectPtr(new COLUMNS(*this));
}


// COLUMNS = DefColWidth *255ColInfo
const bool COLUMNS::loadContent(BinProcessor& proc)
{
	global_info_ = proc.getGlobalWorkbookInfo();

	bool def_ok = proc.optional<DefColWidth>(); 

	if (def_ok)
	{
		m_DefColWidth = elements_.back();
		elements_.pop_back();
	}
	int count = (global_info_->Version == 0x0200) ? proc.repeated<ColWidth>(0, 255) : proc.repeated<ColInfo>(0, 255);

	int last_add = 0;

	for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); ++it)
	{
		ColInfo* column_info = dynamic_cast<ColInfo*>(it->get());

		for (int i = column_info->colFirst; i <= column_info->colLast; i++)
		{
			global_info_->sheets_info[global_info_->current_sheet - 1].customColumnsWidth.insert(std::make_pair(i, column_info->coldx / 256.));
			//else if (def_ok)
			//{
			//	global_info_->sheets_info.back().customColumnsWidth.insert(std::make_pair(i, global_info_->sheets_info.back().defaultColumnWidth));
			//}
		}

	}

	return def_ok || (count > 0);
}

int COLUMNS::serialize(std::wostream & stream)
{
	if (elements_.size() < 1) return 0;

	CP_XML_WRITER(stream)    
    {
		CP_XML_NODE(L"cols")
		{
			for (std::list<XLS::BaseObjectPtr>::iterator it = elements_.begin(); it != elements_.end(); ++it)
			{
				ColInfo* column_info = dynamic_cast<ColInfo*>(it->get());

				CP_XML_NODE(L"col")
				{
					if (column_info->coldx > 0)
					{
						CP_XML_ATTR(L"width", column_info->coldx / 256.);
						CP_XML_ATTR(L"customWidth", true);
					}

					CP_XML_ATTR(L"min", column_info->colFirst + 1);/// from 0 
					CP_XML_ATTR(L"max", column_info->colLast + 1);

					if (column_info->ixfe > global_info_->cellStyleXfs_count && column_info->ixfe != 0xffff)
					{
						CP_XML_ATTR(L"style", column_info->ixfe - global_info_->cellStyleXfs_count);
					}

					if (column_info->fBestFit)
					{
						CP_XML_ATTR(L"bestFit", column_info->fBestFit);
					}
					if (column_info->fHidden)
					{
						CP_XML_ATTR(L"hidden", column_info->fHidden);
					}
					if (column_info->iOutLevel>0)
					{
						CP_XML_ATTR(L"outlineLevel", column_info->iOutLevel);
					}
				}	
			}
		}
	}
	return 0;
}


} // namespace XLS

