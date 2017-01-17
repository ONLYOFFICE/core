/*
 * (c) Copyright Ascensio System SIA 2010-2017
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
    if (proc.mandatory(globals)) // not dialog
	{
		m_GLOBALS = elements_.back();
		elements_.pop_back();
	}
	int count = 0;

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
    if (proc.mandatory(objects))
	{
		m_OBJECTS = elements_.back();
		elements_.pop_back();
	}

	proc.repeated<HFPicture>(0, 0);
	proc.repeated<Note>(0, 0);
	proc.optional<DCON>();
	proc.repeated<WINDOW>(1, 0);
	
	count = proc.repeated<CUSTOMVIEW>(0, 0);
	while(count > 0)
	{
		m_arCUSTOMVIEW.insert(m_arCUSTOMVIEW.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
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

