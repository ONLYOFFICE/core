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
	if (false == ptg_stack.empty())
	{
		ptg_stack.top() = func_name + L'(' + ptg_stack.top() + L')';
	}
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

void Ptg::save(CFRecord& record)
{
	offset_in_record = record.getRdPtr();
	if(ptg_id.get() < 256)
	{
		unsigned char ptg_1b = ptg_id.get();
		record << ptg_1b;
	}
	else
	{
		unsigned short ptg_2b = ptg_id.get();
		record << ptg_2b;
	}

	writeFields(record);
	size_of_struct = record.getRdPtr() - offset_in_record.get_value_or(0);
}



} // namespace XLS

