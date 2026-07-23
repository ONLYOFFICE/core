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

#include "SXDtr.h"
#include "../../../../../OOXML/Binary/Sheets/Reader/CellFormatController/DateReader.h"

namespace XLS
{

SXDtr::SXDtr()
{
}

SXDtr::~SXDtr()
{
}

BaseObjectPtr SXDtr::clone()
{
	return BaseObjectPtr(new SXDtr(*this));
}

void SXDtr::readFields(CFRecord& record)
{
	record >> yr >> mon >> dom >> hr >> min >> sec;
}

void SXDtr::writeFields(CFRecord& record)
{
	record << yr << mon << dom << hr << min << sec;
}
	
std::wstring SXDtr::value()
{
	if (mon < 1 || mon > 12) mon = 1;
	if (dom < 1 || dom > 31) dom = 1;
	
	if (yr < 1) yr = 1; //???

	std::wstringstream s;
	s << yr << L"-" << (mon < 10 ? L"0" : L"") << mon << L"-" <<  (dom < 10 ? L"0" : L"") << dom << L"T"
		 << (hr < 10 ? L"0" : L"") << hr << L":" << (min < 10 ? L"0" : L"") << min << L":" << (sec < 10 ? L"0" : L"") << sec;

	return s.str();
}

void SXDtr::fromString(const std::wstring &strDate)
{
	DateReader reader;
	tm dateTime;
	if(reader.parseIsoDate(strDate, dateTime))
	{
		yr = dateTime.tm_year + 1900;
		mon = dateTime.tm_mon + 1;
		dom = dateTime.tm_mday + 1;
		hr = dateTime.tm_hour;
		min = dateTime.tm_min;
		sec = dateTime.tm_sec;
	}
}

} // namespace XLS

