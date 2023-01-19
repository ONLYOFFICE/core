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

#include "PtgList.h"
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
    //formula += link; пока без названия sheet
    formula += tableName;
    formula += L'[';
    if(!invalid && !nonresident)
    {
        switch (rowType)
        {
            case 0x00:
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
                formula += L",[" + arrColumn->second[colFirst] + L"]"; if(columns == 0x01) break;
                formula += L":[" + arrColumn->second[colLast] + L"]"; break;
            }
        }
    }

    formula += L']';

    ptg_stack.push(formula);
}

} // namespace XLS

