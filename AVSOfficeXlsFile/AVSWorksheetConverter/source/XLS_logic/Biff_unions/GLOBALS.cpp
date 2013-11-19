#include "stdafx.h"
#include "GLOBALS.h"
#include <XLS_logic/Biff_records/CalcMode.h>
#include <XLS_logic/Biff_records/CalcCount.h>
#include <XLS_logic/Biff_records/CalcRefMode.h>
#include <XLS_logic/Biff_records/CalcIter.h>
#include <XLS_logic/Biff_records/CalcDelta.h>
#include <XLS_logic/Biff_records/CalcSaveRecalc.h>
#include <XLS_logic/Biff_records/PrintRowCol.h>
#include <XLS_logic/Biff_records/PrintGrid.h>
#include <XLS_logic/Biff_records/GridSet.h>
#include <XLS_logic/Biff_records/Guts.h>
#include <XLS_logic/Biff_records/DefaultRowHeight.h>
#include <XLS_logic/Biff_records/WsBool.h>
#include <XLS_logic/Biff_records/Sync.h>
#include <XLS_logic/Biff_records/LPr.h>
#include <XLS_logic/Biff_records/HorizontalPageBreaks.h>
#include <XLS_logic/Biff_records/VerticalPageBreaks.h>

namespace XLS
{;


GLOBALS::GLOBALS(const bool is_dialog_sheet)
:	is_dialog(is_dialog_sheet)
{
}


GLOBALS::~GLOBALS()
{
}


BaseObjectPtr GLOBALS::clone()
{
	return BaseObjectPtr(new GLOBALS(*this));
}


/*
GLOBALS = CalcMode CalcCount CalcRefMode CalcIter CalcDelta CalcSaveRecalc PrintRowCol PrintGrid GridSet 
			Guts DefaultRowHeight WsBool [Sync] [LPr] [HorizontalPageBreaks] [VerticalPageBreaks]
*/
const bool GLOBALS::loadContent(BinProcessor& proc)
{
// 	if(!proc.mandatory<CalcMode>())
// 	{
// 		return false;
// 	}
	proc.optional<CalcMode>(); // OpenOffice Calc stored files workaround (CalcMode is mandatory according to [MS-XLS])
	proc.mandatory<CalcCount>();
	proc.mandatory<CalcRefMode>();
	proc.mandatory<CalcIter>();
	proc.mandatory<CalcDelta>();
	proc.mandatory<CalcSaveRecalc>();
	proc.optional<PrintRowCol>();// OpenOffice Calc stored files workaround (PrintRowCol is mandatory according to [MS-XLS])
	proc.optional<PrintGrid>();// OpenOffice Calc stored files workaround (PrintGrid is mandatory according to [MS-XLS])
	proc.optional<GridSet>();// OpenOffice Calc stored files workaround (GridSet is mandatory according to [MS-XLS])
	proc.mandatory<Guts>();
	proc.mandatory<DefaultRowHeight>();
	proc.mandatory(WsBool(is_dialog)); // The most interesting
	proc.optional<Sync>();
	proc.optional<LPr>();
	proc.optional<HorizontalPageBreaks>();
	proc.optional<VerticalPageBreaks>();
	proc.optional<PrintRowCol>();// OpenOffice Calc stored files workaround (PrintRowCol is mandatory according to [MS-XLS])
	proc.optional<PrintGrid>();// OpenOffice Calc stored files workaround (PrintGrid is mandatory according to [MS-XLS])
	proc.optional<GridSet>();// OpenOffice Calc stored files workaround (GridSet is mandatory according to [MS-XLS])
	return true;
}

} // namespace XLS

