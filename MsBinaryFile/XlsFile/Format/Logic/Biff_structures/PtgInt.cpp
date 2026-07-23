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

#include "PtgInt.h"

namespace XLS
{

PtgInt::PtgInt()
{
}

PtgInt::PtgInt(const std::wstring& word) : OperandPtg(fixed_id)
{
#if defined(_WIN32) || defined (_WIN64)
    value_ = _wtoi(word.c_str());
#else
     wchar_t * pEnd;
    value_ = wcstol(word.c_str(), &pEnd ,10);
#endif
}


BiffStructurePtr PtgInt::clone()
{
	return BiffStructurePtr(new PtgInt(*this));
}


void PtgInt::loadFields(CFRecord& record)
{
	if (record.checkFitReadSafe(2))
	{
		unsigned short tmp;
		record >> tmp;
		value_ = tmp;
	}
}

void PtgInt::writeFields(CFRecord& record)
{
	if (record.checkFitWriteSafe(2))
	{
		unsigned short tmp;
		tmp = value_.get();
		record << tmp;
	}
}

void PtgInt::assemble(AssemblerStack& ptg_stack, PtgQueue& extra_data, bool full_ref)
{
	if (value_)	ptg_stack.push(STR::int2wstr(*value_));
	else		ptg_stack.push(L"");
}


} // namespace XLS

