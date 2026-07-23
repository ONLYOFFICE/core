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

#include "LongRGB.h"

namespace XLS
{


LongRGBA::LongRGBA() : is_alpha(true)
{
}


LongRGBA::LongRGBA(const bool alpha_present) : is_alpha(alpha_present)
{
}


LongRGB::LongRGB() : LongRGBA(false)
{
}

BiffStructurePtr LongRGBA::clone()
{
	return BiffStructurePtr(new LongRGBA(*this));
}


BiffStructurePtr LongRGB::clone()
{
	return BiffStructurePtr(new LongRGB(*this));
}

void LongRGBA::load(CFRecord& record)
{
	record >> red >> green >> blue;
	if(is_alpha)
	{
		record >> alpha;
	}
	else
	{
		alpha = 0;
		record.skipNunBytes(1); // reserved
	}

	strRGB	= STR::toRGB (red, green, blue);
	strARGB	= STR::toARGB(red, green, blue, alpha);
}

void LongRGBA::save(CFRecord& record)
{
	record << red << green << blue << alpha;
}

_UINT32 LongRGBA::ToRGBA()
{
	return ((_UINT32)( ( (unsigned char)(red) )| ( ( (unsigned char)(green) ) << 8 ) | ( ( (unsigned char)(blue) ) << 16 ) | ( (unsigned char)(alpha) << 24 ) ) );
}
_UINT32 LongRGBA::ToRGB()
{
	return ((_UINT32)(((unsigned char)(red)) | (((unsigned char)(green)) << 8) | (((unsigned char)(blue)) << 16) ));
}
static int HexToInt(int nHex)
{
	if (nHex >= '0' && nHex <= '9') return (nHex - '0');
	if (nHex >= 'a' && nHex <= 'f') return (nHex - 'a' + 10);
	if (nHex >= 'A' && nHex <= 'F') return (nHex - 'A' + 10);

	return 0;
}
void LongRGBA::Parse(std::wstring sValue)
{
	if (0 == sValue.find(L"#"))
	{
		sValue = sValue.substr(1);
	}
	int nValueLength = (int)sValue.length();

	if (3 == nValueLength)
	{
		int nTempR = HexToInt((int)sValue[0]);
		int nTempG = HexToInt((int)sValue[1]);
		int nTempB = HexToInt((int)sValue[2]);

		red = nTempR + (unsigned char)(nTempR << 4);
		green = nTempG + (unsigned char)(nTempG << 4);
		blue = nTempB + (unsigned char)(nTempB << 4);
	}
	else if (6 == nValueLength)
	{
		red = HexToInt((int)sValue[1]) + (unsigned char)(HexToInt((int)sValue[0]) << 4);
		green = HexToInt((int)sValue[3]) + (unsigned char)(HexToInt((int)sValue[2]) << 4);
		blue = HexToInt((int)sValue[5]) + (unsigned char)(HexToInt((int)sValue[4]) << 4);
	}
	else if (8 == nValueLength)
	{
		alpha = HexToInt((int)sValue[1]) + (unsigned char)(HexToInt((int)sValue[0]) << 4);
		red = HexToInt((int)sValue[3]) + (unsigned char)(HexToInt((int)sValue[2]) << 4);
		green = HexToInt((int)sValue[5]) + (unsigned char)(HexToInt((int)sValue[4]) << 4);
		blue = HexToInt((int)sValue[7]) + (unsigned char)(HexToInt((int)sValue[6]) << 4);
	}
}

} // namespace XLS
