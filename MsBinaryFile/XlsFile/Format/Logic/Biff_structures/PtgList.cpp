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

#include "PtgList.h"
#include "PtgArea3d.h"
#include "PtgExtraList.h"
#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

PtgList::PtgList(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}

BiffStructurePtr PtgList::clone()
{
        return BiffStructurePtr(new PtgList(*this));
}


void PtgList::loadFields(CFRecord& record)
{	
    //record.skipNunBytes(1); // eptg Reserved
    global_info = record.getGlobalWorkbookInfo();
    record >> ixti;

    unsigned short flags;
    record >> flags;

    columns            = GETBITS(flags, 0, 1);
    rowType            = GETBITS(flags, 2, 6);
    squareBracketSpace = GETBIT(flags, 7);
    commaSpace         = GETBIT(flags, 8);
    type_              = GETBITS(flags, 10, 11);
    invalid            = GETBIT(flags, 12);
    nonresident        = GETBIT(flags, 13);

    record >> listIndex >> colFirst >> colLast;
}

void PtgList::writeFields(CFRecord& record)
{
	//record.skipNunBytes(1); // eptg Reserved
	global_info = record.getGlobalWorkbookInfo();
	record << ixti;
	unsigned short flags = 0;

	SETBITS(flags, 0, 1, columns)
	SETBITS(flags, 2, 6, rowType)
	SETBIT(flags, 7, squareBracketSpace)
	SETBIT(flags, 8, commaSpace)
	SETBITS(flags, 10, 11, type_)
	SETBIT(flags, 12, invalid)
	SETBIT(flags, 13, nonresident)

	record << flags;
	record << listIndex << colFirst << colLast;
}

void PtgList::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
    //ptg_stack.push(L""); tblExpenses[[#This Row],[Hotel]:[Transport]]
    std::wstring formula = L"";
    std::wstring link = L"";
    std::wstring row = L"";
    std::wstring tableName = L"";
    auto tableIndex = global_info->mapTableNames.find(listIndex);
    if (tableIndex != global_info->mapTableNames.end())
    {
        tableName = tableIndex->second;
    }
    else if(nonresident && !extra_data.empty())
    {
        auto extraList = static_cast<PtgExtraList*>(extra_data.front().get());
        tableName = extraList->table;
    }
    else
    {
        tableName = XmlUtils::GenerateGuid();
        global_info->mapTableGuidsIndex.insert({tableName, listIndex});
    }

    if (ixti < global_info->arXti_External.size())
    {
        link = global_info->arXti_External[ixti].link;
        if (!link.empty())
            link += L"!";
    }
    //formula += link; for now without sheet name
    formula += tableName;
    formula += L'[';
    if(!invalid && !nonresident)
    {
        switch (rowType)
        {
            case 0x04: formula += L"[#Data]"; break;
            case 0x01: formula += L"[#All]"; break;
            case 0x02: formula += L"[#Headers]"; break;
            case 0x06: formula += L"[#Headers],[#Data]"; break;
            case 0x08: formula += L"[#Totals]"; break;
            case 0x0C: formula += L"[#Data],[#Totals]"; break;
            case 0x10: formula += L"[#This Row]"; break;
        }

        auto arrColumn = global_info->mapTableColumnNames.find(listIndex);
        if (arrColumn != global_info->mapTableColumnNames.end())
        {
            switch (columns)
            {
                case 0x00: break;
                case 0x01:
                case 0x02:
                if(colFirst >= arrColumn->second.size())
                    break;
                if(rowType != 0x00)
                    formula += L",";
                formula += L"['" + arrColumn->second[colFirst] + L"]";
                if(columns == 0x01) break;
                if(colLast >= arrColumn->second.size())
                    break;
                formula += L":[" + arrColumn->second[colLast] + L"]"; break;
            }
        }
    }

    formula += L']';

    ptg_stack.push(formula);
}

Ptg* PtgList::toArea()
{
	Ref8 tableRef;
	auto tableRefIndex = XLS::GlobalWorkbookInfo::mapTableRefsStatic.find(listIndex);
	if(tableRefIndex != XLS::GlobalWorkbookInfo::mapTableRefsStatic.end())
	{
		tableRef.fromString(std::get<0>(tableRefIndex->second));
		tableRef.columnFirst += colFirst;
		tableRef.columnLast = tableRef.columnFirst;
		if(colLast > colFirst)
			tableRef.columnLast += (colLast - colFirst);
		if(rowType == 0x2) //headers
			tableRef.rowLast = tableRef.rowFirst;
		else if(rowType == 0x0) //data
		{
			tableRef.rowFirst+= std::get<1>(tableRefIndex->second);
			tableRef.rowLast-= std::get<2>(tableRefIndex->second);
		}
		else if(rowType == 0x6) //dataheaders
			tableRef.rowLast-= std::get<2>(tableRefIndex->second);
		else if(rowType == 0x0C) // datatotals
			tableRef.rowFirst+= std::get<1>(tableRefIndex->second);
		else if(rowType == 0x8) //totals
			tableRef.rowFirst = tableRef.rowLast;
	}
	unsigned char ptgType = type_+1;
	unsigned char areaType = 0x3B;
	SETBITS(areaType,5,6, ptgType);

	PtgArea3d* listArea = new PtgArea3d(areaType, CellRef());
	listArea->ixti = ixti;
	listArea->area = tableRef;
	listArea->area.rowFirstRelative = false;
	listArea->area.rowLastRelative = false;
	listArea->area.columnFirstRelative = false;
	listArea->area.columnLastRelative = false;
	Ptg* ptr = listArea;

	return ptr;
}

} // namespace XLS

