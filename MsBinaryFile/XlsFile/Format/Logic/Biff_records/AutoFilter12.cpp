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

#include "AutoFilter12.h"
#include "../Biff_structures/DXFN12List.h"

#include "../Biff_structures/BiffString.h"

#include "../Biff_structures/AFDOper.h"
#include "../Biff_structures/AF12Criteria.h"
#include "../Biff_structures/AF12CellIcon.h"

namespace XLS
{

AutoFilter12::AutoFilter12()
{
}


AutoFilter12::~AutoFilter12()
{
}


BaseObjectPtr AutoFilter12::clone()
{
	return BaseObjectPtr(new AutoFilter12(*this));
}


void AutoFilter12::readFields(CFRecord& record)
{
	_UINT16 flags;
	_UINT32 unused2;
	
	record >> frtRefHeader >> iEntry >> fHideArrow >> ft >> cft >> cCriteria >> cDateGroupings >> flags >> unused2 >> idList;

	_GUID_ guid_num;
	record >> guid_num;
	guidSview = STR::guid2bstr(guid_num);

	record.skipNunBytes(record.getDataSize() - record.getRdPtr());
//-------------------------------------------------------------------------------------------------------
	std::list<CFRecordPtr>& recs = continue_records[rt_ContinueFrt12];
	size_t size = recs.size();

	switch(ft)
	{
	case 0x00000000://not exist
		break;
	case 0x00000001: rgb = BiffStructurePtr(new DXFN12List); //color
		break;
	case 0x00000002: rgb = BiffStructurePtr(new DXFN12List); //font
		break;
	case 0x00000003: rgb = BiffStructurePtr(new AF12CellIcon); //icon
		break;
	}
	if (rgb)
		rgb->load(record);

	while( !recs.empty() )
	{
		record.appendRawData(recs.front());
		recs.pop_front();

		//
		FrtRefHeader frtRefHeaderContinue;
		record >> frtRefHeaderContinue;
		if (frtRefHeaderContinue.rt == 0x087F)
		{		
			if ( ft == 0 && cCriteria > 0)
			{
				AF12CriteriaPtr item(new AF12Criteria);
				item->load(record);
				
				arAF12Criteries.push_back(item);
			}
		}		
	}
}

void AutoFilter12::writeFields(CFRecord& record)
{
	_UINT16 flags = 0;
	frtRefHeader.rt = 0x087E;
	record << frtRefHeader << iEntry << fHideArrow << ft << cft << cCriteria << cDateGroupings;
	bool fWorksheetAutoFilter = false;
	if(idList == 0xFFFFFFFF)
		fWorksheetAutoFilter = true;
	SETBIT(flags, 3, fWorksheetAutoFilter)
	record << flags;
	record.reserveNunBytes(4);
	record << idList;

	_GUID_ guid_num;
	if(!guidSview.empty())
		STR::bstr2guid(guidSview, guid_num);
	record << guid_num;
	if(rgb != nullptr)
		rgb->save(record);

}

} // namespace XLS

