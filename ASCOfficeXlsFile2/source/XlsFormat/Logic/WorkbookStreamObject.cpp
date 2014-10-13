#include "precompiled_xls.h"
#include "WorkbookStreamObject.h"
#include <Binary/CFStream.h>
#include <Binary/CFRecord.h>
#include "AnyObject.h"
//#include <Logic/AnySubstream.h>
#include "GlobalsSubstream.h"
#include "WorksheetSubstream.h"
#include "MacroSheetSubstream.h"
#include "ChartSheetSubstream.h"
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_records/EOF.h>
//#include <Exception/EndOfStreamReached.h>

namespace XLS
{;

WorkbookStreamObject::WorkbookStreamObject()
: code_page_(DefaultCodePage)
{
}


WorkbookStreamObject::WorkbookStreamObject(const unsigned __int16 code_page)
: code_page_(code_page)
{
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

	/*try
	{
		while(1)
		{
			proc.mandatory<AnyObject>();
		}
	}
	catch(EXCEPT::RT::EndOfStreamReached&)
	{
	}
	return true;*/


	bool to_continue = true;

	unsigned __int16 substream_type;
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
				if(proc.mandatory(GlobalsSubstream(code_page_)))
				{
					GlobalsSubstream_found = true;
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
				if(proc.mandatory(WorksheetSubstream(ws_index++)))
				{
					WorksheetSubstream_found = true;
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
				if(proc.mandatory<ChartSheetSubstream>())
				{
					WorksheetSubstream_found = true;
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
				if(proc.mandatory<MacroSheetSubstream>())
				{
					WorksheetSubstream_found = true;
				}
			}
			break;
			default:
				Log::warning("WARNING: Substream of unsupported type " + STR::int2str(substream_type, 10) +
					"  The substream is skipped! Sorry.");
				proc.SeekToEOF();
				proc.mandatory<EOF_T>();
				//throw XLS::WrongBiffRecord("Substream of unsupported type " + STR::int2str(substream_type, 10), "BOF");
		
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
