#include "stdafx.h"
#include "WorksheetSubstream.h"
#include <XLS_logic/Biff_records/Uncalced.h>
#include <XLS_logic/Biff_records/Index.h>
#include <XLS_logic/Biff_unions/GLOBALS.h>
#include <XLS_logic/Biff_unions/PAGESETUP.h>
//#include <XLS_logic/Biff_records/HeaderFooter.h> // Moved inside PAGESETUP
#include <XLS_logic/Biff_records/Dimensions.h>
#include <XLS_logic/Biff_records/HFPicture.h>
#include <XLS_logic/Biff_records/Note.h>
#include <XLS_logic/Biff_records/DxGCol.h>
#include <XLS_logic/Biff_records/MergeCells.h>
#include <XLS_logic/Biff_records/LRng.h>
#include <XLS_logic/Biff_records/CodeName.h>
#include <XLS_logic/Biff_records/WebPub.h>
#include <XLS_logic/Biff_records/CellWatch.h>
#include <XLS_logic/Biff_records/SheetExt.h>
#include <XLS_logic/Biff_records/EOF.h>
#include <XLS_logic/Biff_records/BOF.h>
#include <XLS_logic/Biff_unions/BACKGROUND.h>
#include <XLS_logic/Biff_unions/BIGNAME.h>
#include <XLS_logic/Biff_unions/PROTECTION_COMMON.h> // special name to exclude ambiguous meaning
#include <XLS_logic/Biff_unions/COLUMNS.h>
#include <XLS_logic/Biff_unions/SCENARIOS.h>
#include <XLS_logic/Biff_unions/SORTANDFILTER.h>
#include <XLS_logic/Biff_unions/CELLTABLE.h>
#include <XLS_logic/Biff_unions/OBJECTS.h>
#include <XLS_logic/Biff_unions/PIVOTVIEW.h>
#include <XLS_logic/Biff_unions/DCON.h>
#include <XLS_logic/Biff_unions/WINDOW.h>
#include <XLS_logic/Biff_unions/CUSTOMVIEW.h>
#include <XLS_logic/Biff_unions/SORT.h>
#include <XLS_logic/Biff_unions/QUERYTABLE.h>
#include <XLS_logic/Biff_unions/PHONETICINFO.h>
#include <XLS_logic/Biff_unions/CONDFMTS.h>
#include <XLS_logic/Biff_unions/HLINK.h>
#include <XLS_logic/Biff_unions/DVAL.h>
#include <XLS_logic/Biff_unions/FEAT.h>
#include <XLS_logic/Biff_unions/FEAT11.h>
#include <XLS_logic/Biff_unions/RECORD12.h>
#include <XLS_logic/Biff_unions/SHFMLA_SET.h>

namespace XLS
{;


WorksheetSubstream::WorksheetSubstream(const size_t ws_index)
:	ws_index_(ws_index)
{
}


WorksheetSubstream::~WorksheetSubstream()
{
}


BaseObjectPtr WorksheetSubstream::clone()
{
	return BaseObjectPtr(new WorksheetSubstream(*this));
}


/*
WORKSHEETCONTENT = [Uncalced] Index GLOBALS PAGESETUP [HeaderFooter] [BACKGROUND] *BIGNAME [PROTECTION] 
					COLUMNS [SCENARIOS] SORTANDFILTER Dimensions [CELLTABLE] OBJECTS *HFPicture *Note 
					*PIVOTVIEW [DCON] 1*WINDOW *CUSTOMVIEW *2SORT [DxGCol] *MergeCells [LRng] *QUERYTABLE 
					[PHONETICINFO] CONDFMTS *HLINK [DVAL] [CodeName] *WebPub *CellWatch [SheetExt] *FEAT 
					*FEAT11 *RECORD12 EOF
WORKSHEET = BOF WORKSHEETCONTENT
*/
const bool WorksheetSubstream::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<BOF>())
	{
		return false;
	}

	proc.optional<Uncalced>();
	proc.optional<Index>(); // OpenOffice Calc stored files workaround (Index is mandatory according to [MS-XLS])
	proc.mandatory(GLOBALS(false)); // not dialog
	proc.optional<COLUMNS>(); // OpenOffice Calc stored files workaround (DefColWidth is mandatory and located inside COLUMNS according to [MS-XLS])
	proc.mandatory<PAGESETUP>();
	//proc.optional<HeaderFooter>(); // Moved inside PAGESETUP
	proc.optional<BACKGROUND>();
	proc.repeated<BIGNAME>(0, 0);
	proc.optional<PROTECTION_COMMON>();
	proc.optional<COLUMNS>();
	proc.optional<SCENARIOS>();
	proc.optional<SORTANDFILTER>();  // Let it be optional
	proc.mandatory<Dimensions>();

	std::vector<CellRef> shared_formulas_locations;
	proc.optional(CELLTABLE(shared_formulas_locations));
	if(0 != shared_formulas_locations.size())
	{
		proc.optional(SHFMLA_SET(shared_formulas_locations));
	}

	proc.optional(OBJECTS(false));  // Let it be optional
	proc.repeated<HFPicture>(0, 0);
	proc.repeated<Note>(0, 0);
	proc.repeated<PIVOTVIEW>(0, 0);
	proc.optional<DCON>();
	proc.repeated<WINDOW>(1, 0);
	proc.repeated<CUSTOMVIEW>(0, 0);
	proc.repeated<SORT>(0, 2);
	proc.optional<DxGCol>();
	proc.repeated<MergeCells>(0, 0);
	proc.optional<LRng>();
	proc.repeated<QUERYTABLE>(0, 0);
	proc.optional<PHONETICINFO>();
	proc.optional<CONDFMTS>();  // Let it be optional
	proc.repeated<HLINK>(0, 0);
	proc.optional<DVAL>();
	proc.optional<CodeName>();
	proc.repeated<WebPub>(0, 0);
	proc.repeated<CellWatch>(0, 0);
	proc.optional<SheetExt>();
	proc.repeated<FEAT>(0, 0);
	proc.repeated<FEAT11>(0, 0);
	proc.repeated<RECORD12>(0, 0);

#pragma message("####################### Some trash records may be skipped here")
	proc.SeekToEOF(); // Thus we skip problems with the trash at the end of the stream (found in Domens.xls)
	
	proc.mandatory<EOF_T>();

	return true;
}

void WorksheetSubstream::toXML(MSXML2::IXMLDOMElementPtr own_tag)
{
	static _bstr_t index_name("index");
	own_tag->setAttribute(index_name, ws_index_);
}

} // namespace XLS

