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
#include "Format.h"

#include "../../../../../OOXML/Base/Unit.h"

namespace XLS
{

	Format::Format()
	{}
	Format::~Format()
	{}
	BaseObjectPtr Format::clone()
	{
		return BaseObjectPtr(new Format(*this));
	}
	Format_BIFF23::Format_BIFF23()
	{}
	Format_BIFF23::~Format_BIFF23()
	{}
	BaseObjectPtr Format_BIFF23::clone()
	{
		return BaseObjectPtr(new Format_BIFF23(*this));
	}
//-----------------------------------------------------------------------------------
void Format::readFields(CFRecord& record)
{
	GlobalWorkbookInfoPtr global_info = record.getGlobalWorkbookInfo();
	
	ifmt = 0xffff;
	if (global_info->Version > 0x0300)
	{
		if (global_info->Version == 0x0400)
		{
			_UINT16 notUsed;
			record >> notUsed;
		}
		else
		{
			record >> ifmt;
			ifmt_used = ifmt;
		}
	}

	if (global_info->Version < 0x0600)
	{
		ShortXLAnsiString format;
		record >> format;
		
		stFormat = XmlUtils::EncodeXmlString(format.value(), true);
	}
	else
	{
		XLUnicodeString format;
		record >> format;

		stFormat = XmlUtils::EncodeXmlString(format.value(), true);
	}
}

void Format::writeFields(CFRecord& record)
{
    XLUnicodeString format;
    format = stFormat;
    record << ifmt << format;
}

int Format::serialize(std::wostream & stream)
{
	if (ifmt < 5 || (ifmt > 8 && ifmt < 23) || (ifmt > 36 && ifmt < 41) || (ifmt > 44 && ifmt < 50)) return 0;

	stream << L"<numFmt";
	{
		stream << L" numFmtId=\"" << ifmt_used << L"\"";
		stream << L" formatCode=\"" << stFormat << L"\"";
	}
	stream << L"/>";

	return 1;
}

} // namespace XLS

