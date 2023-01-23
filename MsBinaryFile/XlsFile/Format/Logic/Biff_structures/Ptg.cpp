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

#include "Ptg.h"


namespace XLS
{

Ptg::Ptg()
{
}

Ptg::Ptg(const unsigned short ptg_id_init) : ptg_id(ptg_id_init)
{
}

const unsigned short Ptg::getPtgId() const
{
	return ptg_id.get_value_or(0);
}

const size_t Ptg::getOffsetInRecord() const
{
	return offset_in_record.get_value_or(0);
}

const size_t Ptg::getSizeOfStruct() const
{
	return size_of_struct.get_value_or(0);
}

void Ptg::addFuncWrapper(AssemblerStack& ptg_stack, const std::wstring& func_name)
{
	ptg_stack.top() = func_name + L'(' + ptg_stack.top() + L')';
}

void Ptg::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{

}

void Ptg::load(CFRecord& record)
{
	offset_in_record = record.getRdPtr();
	unsigned char short_type;
	record >> short_type;
	ptg_id = short_type;
	if(0x18 == short_type || 0x19 == short_type)
	{
		unsigned char high_part;
		record >> high_part;
		ptg_id = static_cast<unsigned short>(short_type) + (static_cast<unsigned short>(high_part) << 8);
	}
	loadFields(record);
	size_of_struct = record.getRdPtr() - offset_in_record.get_value_or(0);
}



} // namespace XLS

