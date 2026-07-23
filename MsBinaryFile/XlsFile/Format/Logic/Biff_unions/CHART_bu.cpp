/*
 * Copyright (C) Ascensio System SIA, 2009-2026
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation, together with the
 * additional terms provided in the LICENSE file.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. For
 * details, see the GNU AGPL at: https://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA by email at info@onlyoffice.com
 * or by postal mail at 20A-6 Ernesta Birznieka-Upisha Street, Riga,
 * LV-1050, Latvia, European Union.
 *
 * The interactive user interfaces in modified versions of the Program
 * are required to display Appropriate Legal Notices in accordance with
 * Section 5 of the GNU AGPL version 3.
 *
 * No trademark rights are granted under this License.
 *
 * All non-code elements of the Product, including illustrations,
 * icon sets, and technical writing content, are licensed under the
 * Creative Commons Attribution-ShareAlike 4.0 International License:
 * https://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 * This license applies only to such non-code elements and does not
 * modify or replace the licensing terms applicable to the Program's
 * source code, which remains licensed under the GNU Affero General
 * Public License v3.
 *
 * SPDX-License-Identifier: AGPL-3.0-only
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

