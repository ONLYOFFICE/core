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

#include "PtgAreaN.h"
#include "CellRangeRef.h"

namespace XLS
{

PtgAreaN::PtgAreaN(const unsigned short full_ptg_id, const CellRef cell_base_ref_init) : OperandPtg(full_ptg_id), cell_base_ref(cell_base_ref_init)
{
}

PtgAreaN::PtgAreaN(const std::wstring& word, const PtgDataType data_type, const CellRef cell_base_ref_init)
:	OperandPtg(fixed_id | (static_cast<unsigned char>(data_type) << 5)),
	area(word),
    areaXlsb(word),
	cell_base_ref(cell_base_ref_init)
{
	area -= cell_base_ref;
    areaXlsb -= cell_base_ref;
}

void PtgAreaN::set_base_ref(const CellRef& cell_base_ref_new)
{
	area += cell_base_ref;
    areaXlsb += cell_base_ref;
	
	cell_base_ref = cell_base_ref_new;
	
	area -= cell_base_ref;
    areaXlsb -= cell_base_ref;
}


BiffStructurePtr PtgAreaN::clone()
{
	return BiffStructurePtr(new PtgAreaN(*this));
}


void PtgAreaN::loadFields(CFRecord& record)
{
    global_info = record.getGlobalWorkbookInfo();

    if (global_info->Version < 0x600)
	{
		unsigned char	colFirst, colLast;
		_UINT16			rwFirst, rwLast;

		record >> rwFirst >> rwLast >> colFirst >> colLast;

		area.rowFirstRelative		= rwFirst & 0x8000;
		area.columnFirstRelative	= rwFirst & 0x4000;
		
		area.columnFirst			= colFirst;
		area.rowFirst				= rwFirst & 0x3FFF;

		area.rowLastRelative	= rwLast & 0x8000;
		area.columnLastRelative	= rwLast & 0x4000;
		
		area.columnLast			= colLast;
		area.rowLast			= rwLast & 0x3FFF;
	}
    else if (global_info->Version < 0x0800)
    {
        record >> area;
    }

    else
    {
        record >> areaXlsb;
    }
}


void PtgAreaN::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
    if (global_info->Version < 0x0800)
    {
        ptg_stack.push((area + cell_base_ref).toString());
    }

    else
    {
        ptg_stack.push((areaXlsb + cell_base_ref).toString());
    }
}


} // namespace XLS

