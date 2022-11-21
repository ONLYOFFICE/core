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

#include "MacroSheetSubstream.h"

#include "Biff_records/BOF.h"
#include "Biff_records/Uncalced.h"
#include "Biff_records/Index.h"
#include "Biff_records/Intl.h"
#include "Biff_records/HeaderFooter.h"
#include "Biff_records/Dimensions.h"
#include "Biff_records/HFPicture.h"
#include "Biff_records/Note.h"
#include "Biff_records/DxGCol.h"
#include "Biff_records/CodeName.h"
#include "Biff_records/CellWatch.h"
#include "Biff_records/SheetExt.h"
#include "Biff_records/EOF.h"

#include "Biff_unions/MACROSORTANDFILTER.h"
#include "Biff_unions/GLOBALS.h"
#include "Biff_unions/PAGESETUP.h"
#include "Biff_unions/BACKGROUND.h"
#include "Biff_unions/BIGNAME.h"
#include "Biff_unions/PROTECTION_COMMON.h"
#include "Biff_unions/COLUMNS.h"
#include "Biff_unions/CELLTABLE.h"
#include "Biff_unions/OBJECTS.h"
#include "Biff_unions/DCON.h"
#include "Biff_unions/WINDOW.h"
#include "Biff_unions/CUSTOMVIEW.h"
#include "Biff_unions/SORT.h"
#include "Biff_unions/PHONETICINFO.h"
#include "Biff_unions/FEAT.h"
#include "Biff_unions/RECORD12.h"

namespace XLS
{;


MacroSheetSubstream::MacroSheetSubstream(const size_t ws_index) :	CommonSubstream(ws_index)
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
	global_info_ = proc.getGlobalWorkbookInfo();
	global_info_->current_sheet = ws_index_ + 1; 

	proc.optional<Uncalced>();
	proc.mandatory<Index>();
	proc.optional<Intl>();

    if (proc.mandatory<GLOBALS>())
	{
		m_GLOBALS = elements_.back();
		elements_.pop_back();
	}
	int count = 0;

	if (proc.mandatory<PAGESETUP>())
	{
		m_PAGESETUP = elements_.back();
		elements_.pop_back();
	}
	proc.optional<HeaderFooter>();

	if (proc.optional<BACKGROUND>())
	{
		m_BACKGROUND = elements_.back();
		elements_.pop_back();
	}
	proc.repeated<BIGNAME>(0, 0);

	if (proc.optional<PROTECTION_COMMON>())
	{
		m_PROTECTION = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<COLUMNS>())
	{
		m_COLUMNS = elements_.back();
		elements_.pop_back();
	}
	
	if (proc.mandatory<MACROSORTANDFILTER>())
	{
		m_MACROSORTANDFILTER = elements_.back();
		elements_.pop_back();
	}
	if (proc.mandatory<Dimensions>())
	{
		m_Dimensions = elements_.back();
		elements_.pop_back();
	}

	std::vector<CellRangeRef> shared_formulas_locations;   
	CELLTABLE cell_table(shared_formulas_locations);
    if (proc.optional(cell_table))
	{
		m_CELLTABLE = elements_.back();
		elements_.pop_back();
	}

    OBJECTS objects(false);
    if (proc.mandatory(objects))
	{
		m_OBJECTS = elements_.back();
		elements_.pop_back();
	}

	count = proc.repeated<HFPicture>(0, 0);		
	while(count > 0)
	{
		m_arHFPicture.insert(m_arHFPicture.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}

	count = proc.repeated<Note>(0, 0);
	while(count > 0)
	{
		m_arNote.insert(m_arNote.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	if (proc.optional<DCON>())
	{
		m_DCON = elements_.back();
		elements_.pop_back();
	}
	
	count = proc.repeated<WINDOW>(0, 0);
	while(count > 0)
	{
		m_arWINDOW.insert(m_arWINDOW.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}	
	count = proc.repeated<CUSTOMVIEW>(0, 0);
	while(count > 0)
	{
		m_arCUSTOMVIEW.insert(m_arCUSTOMVIEW.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	count = proc.repeated<SORT>(0, 2);
	while(count > 0)
	{
		m_arSORT.insert(m_arSORT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	if (proc.optional<DxGCol>())
	{
		m_DxGCol = elements_.back();
		elements_.pop_back(); 
		
		DxGCol* dx = dynamic_cast<DxGCol*>(m_DxGCol.get());
		global_info_->sheets_info.back().defaultColumnWidth = dx->dxgCol / 256.;
	}
	
	proc.optional<PHONETICINFO>();
	
	if (proc.optional<CodeName>())
	{
		m_CodeName  = elements_.back();
		elements_.pop_back();
	}
	proc.repeated<CellWatch>(0, 0);
	if (proc.optional<SheetExt>())
	{
		m_SheetExt  = elements_.back();
		elements_.pop_back();
	}
	count = proc.repeated<FEAT>	(0, 0);
	while(count > 0)
	{
		m_arFEAT.insert(m_arFEAT.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	count = proc.repeated<RECORD12>	(0, 0);		
	while(count > 0)
	{
		m_arRECORD12.insert(m_arRECORD12.begin(), elements_.back());
		elements_.pop_back();
		count--;
	}
	proc.mandatory<EOF_T>();

	LoadHFPicture();

	return true;
}

} // namespace XLS

