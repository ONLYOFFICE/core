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

#include "RgbExtra.h"
#include "PtgExtraCol.h"
#include "PtgExtraArray.h"
#include "PtgExtraList.h"
#include "PtgExtraMem.h"
#include "PtgExtraElf.h"
#include "RevNameTabid.h"
#include "RevName.h"
#include "RevExtern.h"
#include "Ptg.h"


namespace XLS
{


BiffStructurePtr RgbExtra::clone()
{
	return BiffStructurePtr(new RgbExtra(*this));
}

// This is just a stub for the virtual 'load' realization
void RgbExtra::load(CFRecord& record)
{
}

void RgbExtra::save(CFRecord& record)
{
	while (!ptg_records.empty())
	{
		auto& front = ptg_records.front();
		front->save(record);
		ptg_records.pop();
	}
}


void RgbExtra::load(CFRecord& record, const PtgVector& records_from_rgce, bool is_part_of_a_revision)
{
	for(PtgVectorIterator it = records_from_rgce.begin(), 
		itEnd = records_from_rgce.end(); it != itEnd; ++it)
	{
		load(record, (*it)->getPtgId(), is_part_of_a_revision);
	}
}


void RgbExtra::load(CFRecord& record, unsigned short rgce_record_type, bool is_part_of_a_revision)
{
	PtgPtr extra_ptg;
	switch(rgce_record_type)
	{
        case 0x0001: // PtgExp
            extra_ptg = PtgPtr(new PtgExtraCol);
            break;
        case 0x1918: // PtgList
            extra_ptg = PtgPtr(new PtgExtraList);
            break;
		case 0x0020: // PtgArray type = REFERENCE
		case 0x0040: // PtgArray type = VALUE
		case 0x0060: // PtgArray type = ARRAY
			extra_ptg = PtgPtr(new PtgExtraArray);
			break;
		case 0x0026: // PtgMemArea type = REFERENCE
		case 0x0046: // PtgMemArea type = VALUE
		case 0x0066: // PtgMemArea type = ARRAY
			extra_ptg = PtgPtr(new PtgExtraMem);
			break;
		case 0x180B: // PtgElfRadicalS
		case 0x180D: // PtgElfColS
		case 0x180F: // PtgElfColSV
			extra_ptg = PtgPtr(new PtgExtraElf);
			break;
		case 0x0023: // PtgName type = REFERENCE
		case 0x0043: // PtgName type = VALUE
		case 0x0063: // PtgName type = ARRAY
			if(is_part_of_a_revision)
			{
				extra_ptg = PtgPtr(new RevNameTabid);
			}
			break;
		case 0x0039: // PtgNameX type = REFERENCE
		case 0x0059: // PtgNameX type = VALUE
		case 0x0079: // PtgNameX type = ARRAY
			if(is_part_of_a_revision)
			{
				extra_ptg = PtgPtr(new RevName);
			}
			break;
		case 0x003A: // PtgRef3d type = REFERENCE
		case 0x005A: // PtgRef3d type = VALUE
		case 0x007A: // PtgRef3d type = ARRAY
		case 0x003C: // PtgRefErr3d type = REFERENCE
		case 0x005C: // PtgRefErr3d type = VALUE
		case 0x007C: // PtgRefErr3d type = ARRAY
		case 0x003B: // PtgArea3d type = REFERENCE
		case 0x005B: // PtgArea3d type = VALUE
		case 0x007B: // PtgArea3d type = ARRAY
		case 0x003D: // PtgAreaErr3d type = REFERENCE
		case 0x005D: // PtgAreaErr3d type = VALUE
		case 0x007D: // PtgAreaErr3d type = ARRAY
			if(is_part_of_a_revision)
			{
				extra_ptg = PtgPtr(new RevExtern);
			}
			break;
		default:
			// we skip the records that shall not have the corresponding record in RgbExtra
			break;
	}
	if(!!extra_ptg)
	{
		extra_ptg->load(record);
		ptg_records.push(extra_ptg);
	}
}


const PtgQueue& RgbExtra::getPtgs() const
{
	return ptg_records;
}


void RgbExtra::addPtg(const PtgPtr ptg)
{
	ptg_records.push(ptg);
}


const bool RgbExtra::isEmpty() const
{
	return ptg_records.empty();
}


} // namespace XLS

