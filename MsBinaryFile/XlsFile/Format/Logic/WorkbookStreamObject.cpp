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

#include "WorkbookStreamObject.h"
#include "../Binary/CFStream.h"
#include "AnyObject.h"

#include "GlobalsSubstream.h"
#include "WorksheetSubstream.h"
#include "MacroSheetSubstream.h"
#include "ChartSheetSubstream.h"
#include "Biff_records/BOF.h"
#include "Biff_records/EOF.h"


namespace XLS
{;

WorkbookStreamObject::WorkbookStreamObject() : code_page_(DefaultCodePage)
{
}


WorkbookStreamObject::WorkbookStreamObject(const unsigned short code_page) : code_page_(code_page)
{
}

void WorkbookStreamObject::set_code_page(const unsigned short code_page) 
{
	code_page_ = code_page;
}

WorkbookStreamObject::~WorkbookStreamObject()
{
}

BaseObjectPtr WorkbookStreamObject::clone()
{
	return BaseObjectPtr(new WorkbookStreamObject(*this));
}

const bool WorkbookStreamObject::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info_ = proc.getGlobalWorkbookInfo();
	
	bool GlobalsSubstream_found		= false;
	bool WorksheetSubstream_found	= false;
	
	size_t ws_index = 0;

	GlobalWorkbookInfo::_sheet_info sheet_info;

	while(true)
	{
		unsigned short substream_type = 0;
		
		if (!proc.getNextSubstreamType(substream_type))
			break;

		switch(substream_type)
		{
			case BOF::st_Workbook:
			{
				if(GlobalsSubstream_found)
				{
					Log::error("Multiple GLOBALS substreams found in intermediate XML.");
					return false;
				}
				Log::event("Globals substream detected");
                GlobalsSubstream global_substream(code_page_);
                if((proc.mandatory(global_substream)) && (elements_.size() > 0))
				{
					GlobalsSubstream_found = true;
					
					m_GlobalsSubstream = elements_.back(); elements_.pop_back();
				}
				if (!GlobalsSubstream_found) return false;
			}
			break;
			case BOF::st_Worksheet:
			{
				if(!GlobalsSubstream_found)
				{
					Log::error("GLOBALS substream is not the first substream in intermediate XML.");
					return false;
				}
				Log::event("Worksheet or Dialog substream detected");
                
				if (ws_index >= global_info_->sheets_info.size())
					global_info_->sheets_info.push_back(sheet_info);

				WorksheetSubstream worksheet_substream(ws_index++);
                if ((proc.mandatory(worksheet_substream)) && (elements_.size() > 0))
				{
					WorksheetSubstream_found = true;
					
					m_arWorksheetSubstream.push_back(elements_.back()); elements_.pop_back();				
				}
			}
			break;
			case BOF::st_Chart:
			{
				if(!GlobalsSubstream_found)
				{
					Log::error("GLOBALS substream is not the first substream in intermediate XML.");
					return false;
				}
				Log::event("Chart substream detected");

				if (ws_index >= global_info_->sheets_info.size())
					global_info_->sheets_info.push_back(sheet_info);

				ChartSheetSubstream chartsheet_substream(ws_index++);
				if ((proc.mandatory(chartsheet_substream))  && (elements_.size() > 0))
				{
					WorksheetSubstream_found = true;

					m_arChartSheetSubstream.push_back(elements_.back()); elements_.pop_back();
				}
			}
			break;
			case BOF::st_Macro:
			{
				if(!GlobalsSubstream_found)
				{
					Log::error("GLOBALS substream is not the first substream in intermediate XML.");
					return false;
				}
				Log::event("Macro substream detected");

				if (ws_index >= global_info_->sheets_info.size())
					global_info_->sheets_info.push_back(sheet_info);

				MacroSheetSubstream macrosheet_substream(ws_index++);
				if ((proc.mandatory(macrosheet_substream)) && (elements_.size() > 0))
				{
					WorksheetSubstream_found = true;

					m_arMacroSheetSubstream.push_back(elements_.back()); elements_.pop_back();
				}
			}
			break;
			default:
				if (substream_type != 0)
				{
					Log::warning("WARNING: Substream of unsupported type " + STR::int2str(substream_type, 10) +
						"  The substream is skipped!");
				}
				proc.SeekToEOF();
				proc.optional<EOF_T>();
		
		}
	}


	if(!GlobalsSubstream_found)
	{
		Log::error("GLOBALS substream hasn't been found in intermediate XML.");
		return false;
	}
	if(!WorksheetSubstream_found)
	{
		Log::error("There is no any sheet substream found in intermediate XML.");
			return false;
	}
	return true;
}

} // namespace XLS
