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

#include "PtgNum.h"
#include "../Biff_structures/Xnum.h"

namespace XLS
{

PtgNum::PtgNum(const unsigned short full_ptg_id) : OperandPtg(full_ptg_id)
{
}


PtgNum::PtgNum(const std::wstring& word) : value_(0), OperandPtg(fixed_id)
{
#if defined(_WIN32) || defined (_WIN64)
    value_ = _wtof(word.c_str());
#else
    wchar_t * pEnd;
    value_ = wcstod(word.c_str(), &pEnd);
#endif
}


BiffStructurePtr PtgNum::clone()
{
	return BiffStructurePtr(new PtgNum(*this));
}

void PtgNum::loadFields(CFRecord& record)
{
	if (record.checkFitReadSafe(8))
	{
		Xnum tmp;
		record >> tmp;

		value_ = tmp.data.value;
	}
}

void PtgNum::writeFields(CFRecord& record)
{
	if (record.checkFitWriteSafe(8))
	{
		Xnum tmp;
		tmp.data.value = value_.get();
		record << tmp;
	}
}

void PtgNum::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	if (value_)	ptg_stack.push(STR::double2str(*value_));
	else		ptg_stack.push(L"");
}


} // namespace XLS

