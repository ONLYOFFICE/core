
#include "WorkbookStreamObject.h"
#include <Binary/CFStream.h>
#include <Binary/CFRecord.h>
#include "AnyObject.h"

#include "GlobalsSubstream.h"
#include "WorksheetSubstream.h"
#include "MacroSheetSubstream.h"
#include "ChartSheetSubstream.h"
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_records/EOF.h>


namespace XLS
{;

WorkbookStreamObject::WorkbookStreamObject()
: code_page_(DefaultCodePage)
{
}


WorkbookStreamObject::WorkbookStreamObject(const unsigned short code_page)
: code_page_(code_page)
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
	bool to_continue = true;

	unsigned short substream_type;
	bool GlobalsSubstream_found = false;
	bool WorksheetSubstream_found = false;
	size_t ws_index = 0;

	// Find all substreams in this stream
	while(proc.getNextSubstreamType(substream_type))
	{

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
					m_GlobalsSubstream = elements_.back();
					elements_.pop_back();
				}
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
                WorksheetSubstream worksheet_substream(ws_index++);
                if ((proc.mandatory(worksheet_substream)) && (elements_.size() > 0))
				{
					WorksheetSubstream_found = true;
					m_arWorksheetSubstream.push_back(elements_.back());
					elements_.pop_back();				
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
				if ((proc.mandatory<ChartSheetSubstream>())  && (elements_.size() > 0))
				{
					WorksheetSubstream_found = true;
					m_arWorksheetSubstream.push_back(elements_.back());
					elements_.pop_back();
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
				if ((proc.mandatory<MacroSheetSubstream>()) && (elements_.size() > 0))
				{
					WorksheetSubstream_found = true;
					m_arMacroSheetSubstream.push_back(elements_.back());
					elements_.pop_back();
				}
			}
			break;
			default:
				Log::warning("WARNING: Substream of unsupported type " + STR::int2str(substream_type, 10) +
					"  The substream is skipped! Sorry.");
				proc.SeekToEOF();
				proc.mandatory<EOF_T>();
		
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
