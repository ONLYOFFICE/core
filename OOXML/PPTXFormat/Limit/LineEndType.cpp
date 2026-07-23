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

#include "LineEndType.h"

namespace PPTX
{
	namespace Limit
	{		
		LineEndType::LineEndType()
		{
			m_strValue = L"none";
		}
		void LineEndType::set(const std::wstring& strValue)
		{
			if ((L"none" == strValue) ||
				(L"arrow" == strValue) ||
				(L"diamond" == strValue) ||
				(L"oval" == strValue) ||
				(L"stealth" == strValue) ||
				(L"triangle" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char LineEndType::GetBYTECode() const
		{
			if (L"none" == m_strValue)
				return 0;
			if (L"arrow" == m_strValue)
				return 1;
			if (L"diamond" == m_strValue)
				return 2;
			if (L"oval" == m_strValue)
				return 3;
			if (L"stealth" == m_strValue)
				return 4;
			if (L"triangle" == m_strValue)
				return 5;
			return 0;
		}
		void LineEndType::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0:
				m_strValue = L"none";
				break;
			case 1:
				m_strValue = L"arrow";
				break;
			case 2:
				m_strValue = L"diamond";
				break;
			case 3:
				m_strValue = L"oval";
				break;
			case 4:
				m_strValue = L"stealth";
				break;
			case 5:
				m_strValue = L"triangle";
				break;
			default:
				break;
			}
		}
	} // namespace Limit
} // namespace PPTX
