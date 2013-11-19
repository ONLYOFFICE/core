#include "stdafx.h"
#include "MacroSheetSubstream.h"
#include <XLS_logic/Biff_records/BOF.h>
#include <XLS_logic/Biff_records/Uncalced.h>
#include <XLS_logic/Biff_records/Index.h>
#include <XLS_logic/Biff_records/Intl.h>
#include <XLS_logic/Biff_records/HeaderFooter.h>
#include <XLS_logic/Biff_records/Dimensions.h>
#include <XLS_logic/Biff_records/HFPicture.h>
#include <XLS_logic/Biff_records/Note.h>
#include <XLS_logic/Biff_records/DxGCol.h>
#include <XLS_logic/Biff_records/CodeName.h>
#include <XLS_logic/Biff_records/CellWatch.h>
#include <XLS_logic/Biff_records/SheetExt.h>
#include <XLS_logic/Biff_records/EOF.h>

#include <XLS_logic/Biff_unions/MACROSORTANDFILTER.h>
#include <XLS_logic/Biff_unions/GLOBALS.h>
#include <XLS_logic/Biff_unions/PAGESETUP.h>
#include <XLS_logic/Biff_unions/BACKGROUND.h>
#include <XLS_logic/Biff_unions/BIGNAME.h>
#include <XLS_logic/Biff_unions/PROTECTION_COMMON.h>
#include <XLS_logic/Biff_unions/COLUMNS.h>
#include <XLS_logic/Biff_unions/CELLTABLE.h>
#include <XLS_logic/Biff_unions/OBJECTS.h>
#include <XLS_logic/Biff_unions/DCON.h>
#include <XLS_logic/Biff_unions/WINDOW.h>
#include <XLS_logic/Biff_unions/CUSTOMVIEW.h>
#include <XLS_logic/Biff_unions/SORT.h>
#include <XLS_logic/Biff_unions/PHONETICINFO.h>
#include <XLS_logic/Biff_unions/FEAT.h>
#include <XLS_logic/Biff_unions/RECORD12.h>

namespace XLS
{;


MacroSheetSubstream::MacroSheetSubstream()
{
}


MacroSheetSubstream::~MacroSheetSubstream()
{
}


BaseObjectPtr MacroSheetSubstream::clone()
{
	return BaseObjectPtr(new MacroSheetSubstream(*this));
}


/*
MACROSHEETCONTENT = [Uncalced] Index [Intl] GLOBALS PAGESETUP [HeaderFooter] [BACKGROUND] *BIGNAME 
					[PROTECTION] COLUMNS MACROSORTANDFILTER Dimensions [CELLTABLE] OBJECTS *HFPicture 
					*Note [DCON] 1*WINDOW *CUSTOMVIEW *2SORT [DxGCol] [PHONETICINFO] [CodeName] 
					*CellWatch [SheetExt] *FEAT *RECORD12 EOF
MACROSHEET = BOF MACROSHEETCONTENT
*/

const bool MacroSheetSubstream::loadContent(BinProcessor& proc)
{
	if(!proc.mandatory<BOF>())
	{
		return false;
	}
	proc.optional<Uncalced>();
	proc.mandatory<Index>();
	proc.optional<Intl>();
	proc.mandatory(GLOBALS(false)); // not dialog
	proc.mandatory<PAGESETUP>();
	proc.optional<HeaderFooter>();
	proc.optional<BACKGROUND>();
	proc.repeated<BIGNAME>(0, 0);
	proc.optional<PROTECTION_COMMON>();
	proc.mandatory<COLUMNS>();
	proc.mandatory<MACROSORTANDFILTER>();
	proc.mandatory<Dimensions>();

	std::vector<CellRef> shared_formulas_locations;
	proc.optional(CELLTABLE(shared_formulas_locations));

	proc.mandatory(OBJECTS(false));
	proc.repeated<HFPicture>(0, 0);
	proc.repeated<Note>(0, 0);
	proc.optional<DCON>();
	proc.repeated<WINDOW>(1, 0);
	proc.repeated<CUSTOMVIEW>(0, 0);
	proc.repeated<SORT>(0, 2);
	proc.optional<DxGCol>();
	proc.optional<PHONETICINFO>();
	proc.optional<CodeName>();
	proc.repeated<CellWatch>(0, 0);
	proc.optional<SheetExt>();
	proc.repeated<FEAT>(0, 0);
	proc.repeated<RECORD12>(0, 0);
	proc.mandatory<EOF_T>();

	return true;
}

} // namespace XLS

