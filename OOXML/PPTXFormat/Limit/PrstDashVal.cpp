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

#include "PrstDashVal.h"

namespace PPTX
{
	namespace Limit
	{		
		PrstDashVal::PrstDashVal()
		{
			m_strValue = L"solid";
		}
		void PrstDashVal::set(const std::wstring& strValue)
		{
			if ((L"dash" == strValue) ||
				(L"dashDot" == strValue) ||
				(L"dot" == strValue) ||
				(L"lgDash" == strValue) ||
				(L"lgDashDot" == strValue) ||
				(L"lgDashDotDot" == strValue) ||
				(L"solid" == strValue) ||
				(L"sysDash" == strValue) ||
				(L"sysDashDot" == strValue) ||
				(L"sysDashDotDot" == strValue) ||
				(L"sysDot" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char PrstDashVal::GetBYTECode() const
		{
			if (L"dash" == m_strValue)
				return 0;
			if (L"dashDot" == m_strValue)
				return 1;
			if (L"dot" == m_strValue)
				return 2;
			if (L"lgDash" == m_strValue)
				return 3;
			if (L"lgDashDot" == m_strValue)
				return 4;
			if (L"lgDashDotDot" == m_strValue)
				return 5;
			if (L"solid" == m_strValue)
				return 6;
			if (L"sysDash" == m_strValue)
				return 7;
			if (L"sysDashDot" == m_strValue)
				return 8;
			if (L"sysDashDotDot" == m_strValue)
				return 9;
			if (L"sysDot" == m_strValue)
				return 10;
			return 6;
		}
		void PrstDashVal::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0: m_strValue = L"dash"; break;
			case 1: m_strValue = L"dashDot"; break;
			case 2: m_strValue = L"dot"; break;
			case 3: m_strValue = L"lgDash"; break;
			case 4: m_strValue = L"lgDashDot"; break;
			case 5: m_strValue = L"lgDashDotDot"; break;
			case 6: m_strValue = L"solid"; break;
			case 7: m_strValue = L"sysDash"; break;
			case 8: m_strValue = L"sysDashDot"; break;
			case 9: m_strValue = L"sysDashDotDot"; break;
			case 10: m_strValue = L"sysDot"; break;
			default: m_strValue = L"solid"; break;
			}
		}
	} // namespace Limit
} // namespace PPTX
