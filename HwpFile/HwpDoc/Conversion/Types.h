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

#ifndef TYPES_H
#define TYPES_H

#include <cwchar>
#include <string>

namespace HWP
{
	struct TBorderLine
	{
		int m_nColor;
		int m_nInnerLineWidth;
		int m_nOuterLineWudth;
		int nLineDistance;
	};

	struct TColor
	{
		unsigned char m_uchRed;
		unsigned char m_uchGreen;
		unsigned char m_uchBlue;

		TColor()
			: m_uchRed(0x00), m_uchGreen(0x00), m_uchBlue(0x00)
		{}

		TColor(int nColor)
		{
			m_uchRed   = (nColor >> 16 ) & 0xFF;
			m_uchGreen = (nColor >> 8  ) & 0xFF;
			m_uchBlue  = (nColor >> 0  ) & 0xFF;
		}

		TColor(unsigned char uchRed, unsigned char uchGreen, unsigned char chBlue)
			: m_uchRed(uchRed), m_uchGreen(uchGreen), m_uchBlue(chBlue)
		{}

		bool operator!=(const TColor& oColor) const
		{
			return m_uchRed != oColor.m_uchRed || m_uchGreen != oColor.m_uchGreen || m_uchBlue != oColor.m_uchBlue;
		}

		std::wstring ToHEX() const
		{
			const int tempLen = 7;
			wchar_t arTemp[tempLen];

			swprintf(arTemp, tempLen, L"%02X%02X%02X", m_uchRed, m_uchGreen, m_uchBlue);

			return std::wstring(arTemp);
		}

	};

	struct TRelationship
	{
		std::wstring m_wsID;
		std::wstring m_wsType;
		std::wstring m_wsTarget;
	};
}

#endif // TYPES_H
