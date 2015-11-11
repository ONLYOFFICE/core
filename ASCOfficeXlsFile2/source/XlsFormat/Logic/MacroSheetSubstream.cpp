
#include "MacroSheetSubstream.h"
#include <Logic/Biff_records/BOF.h>
#include <Logic/Biff_records/Uncalced.h>
#include <Logic/Biff_records/Index.h>
#include <Logic/Biff_records/Intl.h>
#include <Logic/Biff_records/HeaderFooter.h>
#include <Logic/Biff_records/Dimensions.h>
#include <Logic/Biff_records/HFPicture.h>
#include <Logic/Biff_records/Note.h>
#include <Logic/Biff_records/DxGCol.h>
#include <Logic/Biff_records/CodeName.h>
#include <Logic/Biff_records/CellWatch.h>
#include <Logic/Biff_records/SheetExt.h>
#include <Logic/Biff_records/EOF.h>

#include <Logic/Biff_unions/MACROSORTANDFILTER.h>
#include <Logic/Biff_unions/GLOBALS.h>
#include <Logic/Biff_unions/PAGESETUP.h>
#include <Logic/Biff_unions/BACKGROUND.h>
#include <Logic/Biff_unions/BIGNAME.h>
#include <Logic/Biff_unions/PROTECTION_COMMON.h>
#include <Logic/Biff_unions/COLUMNS.h>
#include <Logic/Biff_unions/CELLTABLE.h>
#include <Logic/Biff_unions/OBJECTS.h>
#include <Logic/Biff_unions/DCON.h>
#include <Logic/Biff_unions/WINDOW.h>
#include <Logic/Biff_unions/CUSTOMVIEW.h>
#include <Logic/Biff_unions/SORT.h>
#include <Logic/Biff_unions/PHONETICINFO.h>
#include <Logic/Biff_unions/FEAT.h>
#include <Logic/Biff_unions/RECORD12.h>

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

    GLOBALS globals(false);
    proc.mandatory(globals); // not dialog

	proc.mandatory<PAGESETUP>();
	proc.optional<HeaderFooter>();
	proc.optional<BACKGROUND>();
	proc.repeated<BIGNAME>(0, 0);
	proc.optional<PROTECTION_COMMON>();
	proc.mandatory<COLUMNS>();
	proc.mandatory<MACROSORTANDFILTER>();
	proc.mandatory<Dimensions>();

	std::vector<CellRangeRef> shared_formulas_locations;
    CELLTABLE cell_table(shared_formulas_locations);
    proc.optional(cell_table);

    OBJECTS objects(false);
    proc.mandatory(objects);

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

