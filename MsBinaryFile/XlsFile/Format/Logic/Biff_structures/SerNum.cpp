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

#include "SerNum.h"

namespace XLS
{


SerNum::SerNum()
{
    fixed_type = typeSerNum;
}


SerNum::SerNum(const std::wstring& word)
{
#if defined(_WIN32) || defined (_WIN64)
    xnum = _wtof(word.c_str());
#else
    wchar_t *pEnd;
    xnum = wcstod(word.c_str(), &pEnd);
#endif
}


BiffStructurePtr SerNum::clone()
{
	return BiffStructurePtr(new SerNum(*this));
}


void SerNum::load(CFRecord& record)
{
	record >> xnum;
	// Excel limitations
	constexpr double ExcelMinAbs = 2.229e-308;
	constexpr double ExcelMax = 9.99999999999999e+307;

	if(std::abs(xnum) < ExcelMinAbs && xnum != 0.0)
		xnum = (xnum > 0) ? ExcelMinAbs : -ExcelMinAbs;
	else if(xnum > ExcelMax)
		xnum = ExcelMax;
	else if(xnum < -ExcelMax)
		xnum = -ExcelMax;
}

void SerNum::save(CFRecord& record)
{
    char serType;
    if (record.getGlobalWorkbookInfo()->Version < 0x0800)
        serType = 1;
    else
        serType = 0;

    record << serType << xnum;
}

const std::wstring SerNum::toString() const
{
	auto tempNum = STR::double2str(xnum);
	if(tempNum == L"-nan")
		tempNum = L"#NUM!";
	return tempNum;
}


} // namespace XLS
