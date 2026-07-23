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

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <iomanip>
#include <sstream>

namespace HWP
{
namespace Transform
{
	inline int HWPINT2OOXML(int nValue)
	{
		return static_cast<int>((double)nValue * 21000. / 59529.);
	}
	inline int HWPUINT2OOXML(int nValue)
	{
		return static_cast<int>((double)nValue * 127.); // 914400. / 7200. = 127
	}

	inline int HWPUINT2Twips(int nValue)
	{
		return static_cast<int>((double)nValue / 7200. * 1440.);
	}

	inline std::wstring IntColorToHEX(int nColor)
	{
		wchar_t buffer[7];
		std::swprintf(buffer, sizeof(buffer) / sizeof(wchar_t), L"%06X", nColor & 0xFFFFFF);
		return std::wstring(buffer);
	}

	inline short LineWidth2Pt(short shHWPThick)
	{
		//TODO:: Verify
		// return (short)std::ceil((double)shHWPThick * 4 * 25.4 / 72.);

		switch(shHWPThick)
		{
			case 0: // 0.1mm
				return 2;
			case 1: // 0.12mm
				return 3;
			case 2: // 0.15mm
				return 4;
			case 3: // 0.2mm
				return 5;
			case 4: // 0.25mm
				return 6;
			case 5: // 0.3mm
				return 7;
			case 6: // 0.4mm
				return 9;
			case 7: // 0.5mm
				return 11;
			case 8: // 0.6mm
				return 14;
			case 9: // 0.7mm
				return 16;
			case 10: // 1.0mm
				return 23;
			case 11: // 1.5mm
				return 34;
			case 12: // 2.0mm
				return 46;
			case 13: // 3.0mm
				return 69;
			case 14: // 4.0mm
				return 92 ;
			case 15: // 5.0mm
				return 115;
			default:
				return 0;
		}
	}
}
}

#endif // TRANSFORM_H
