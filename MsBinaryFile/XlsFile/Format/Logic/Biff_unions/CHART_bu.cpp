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

#include "CHART.h"
#include "../ChartSheetSubstream.h"
#include "../Biff_records/BOF.h"
#include "../Biff_records/Continue.h"
#include "../Biff_unions/CELLTABLE.h"
#include "../Biff_structures/CellRangeRef.h"
#include "../Biff_records/EOF.h"

namespace XLS
{

CHART::~CHART()
{
}

// CHART = CHARTSHEET *Continue

BaseObjectPtr CHART::clone()
{
	return BaseObjectPtr(new CHART(*this));
}

const bool CHART::loadContent(BinProcessor& proc)
{
	GlobalWorkbookInfoPtr global_info = proc.getGlobalWorkbookInfo();

	unsigned short ss_type;
	if(!proc.getNextSubstreamType(ss_type) || BOF::st_Chart != ss_type)
	{
		return false;
	}

	size_t tmp_index_table = global_info->sheets_info.size(); //if present table

	ChartSheetSubstream chart_sheet(tmp_index_table); 
	if(!proc.mandatory(chart_sheet))
	{
		return false;
	}

	m_ChartSheetSubstream = elements_.back();
	elements_.pop_back();

	int count = proc.repeated<Continue>(0, 0);

	while (count > 0)
	{
		Continue* c = dynamic_cast<Continue*>(elements_.back().get());
		if (c)
		{
			CFRecordPtr r = CFRecordPtr(new CFRecord(CFRecordType::ANY_TYPE, proc.getGlobalWorkbookInfo()));
			r->appendRawData(c->m_pData, c->m_iDataSize);
			mso_drawing_->storeRecordAndDecideProceeding(r);

			sp_enabled = true;
			
		}
		elements_.pop_back();
		count--;
	}
//chart_sourcedata_missing_import_biff5.xls
	if (proc.getGlobalWorkbookInfo()->Version < 0x0600)	
	{
		std::vector<CellRangeRef> shared_formulas_locations;
		CELLTABLE cell_table_temlate(shared_formulas_locations);
		
		if (proc.optional(cell_table_temlate))
		{
			//cash table
		}
		if (proc.optional<EOF_T>())
		{//???
		}
	}
	return true;
}

} // namespace XLS

