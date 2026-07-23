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

#include "FileMoniker.h"

namespace OSHARED
{


FileMoniker::FileMoniker()
{
}


FileMoniker::FileMoniker(XLS::CFRecord& record)
{
	load(record);
}


XLS::BiffStructurePtr FileMoniker::clone()
{
	return XLS::BiffStructurePtr(new FileMoniker(*this));
}

void FileMoniker::load(XLS::CFRecord& record)
{
	_UINT32 ansiLength;
	
	record >> cAnti >> ansiLength >> ansiPath >> endServer >> versionNumber;
	record.skipNunBytes(20); // reserved
	record >> cbUnicodePathSize;
	if(cbUnicodePathSize)
	{
		_UINT32 cbUnicodePathBytes;
		record >> cbUnicodePathBytes >> usKeyValue;

#if defined(_WIN32) || defined(_WIN64)
        unicodePath = std::wstring(record.getCurData<wchar_t>(), cbUnicodePathBytes / 2);
#else
        unicodePath = convertUtf16ToWString(record.getCurData<UTF16>(), cbUnicodePathBytes / 2);
#endif
		record.skipNunBytes(cbUnicodePathBytes);
	}
}

void FileMoniker::save(XLS::CFRecord& record)
{
    _UINT32 ansiLength = ansiPath.size();
    record << cAnti << ansiLength << ansiPath << endServer << versionNumber;
    record.reserveNunBytes(20);
    if(!unicodePath.empty())
    {
        cbUnicodePathSize = (unicodePath.size() * 2) + 6;
        _UINT32 cbUnicodePathBytes = (unicodePath.size() * 2);
        record << cbUnicodePathSize << cbUnicodePathBytes << usKeyValue;
        for(auto i : unicodePath)
            record << i;

    }
    else
        record.reserveNunBytes(4);



}

} // namespace OSHARED

