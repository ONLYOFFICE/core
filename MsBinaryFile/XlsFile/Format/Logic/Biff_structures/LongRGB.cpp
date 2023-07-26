/*
 * (c) Copyright Ascensio System SIA 2010-2023
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
 * You can contact Ascensio System SIA at 20A-6 Ernesta Birznieka-Upish
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
