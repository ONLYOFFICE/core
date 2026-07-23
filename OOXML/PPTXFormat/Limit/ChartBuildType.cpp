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

#include "ChartBuildType.h"

namespace PPTX
{
	namespace Limit
	{		
		ChartBuildType::ChartBuildType()
		{
			m_strValue = L"allAtOnce";
		}
		void ChartBuildType::set(const std::wstring& strValue)
		{
			if ((L"allAtOnce" == strValue) ||
				(L"category" == strValue) ||
				(L"categoryEl" == strValue) ||
				(L"series" == strValue) ||
				(L"seriesEl" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char ChartBuildType::GetBYTECode() const
		{
			if (L"allAtOnce" == m_strValue)	return 0;
			if (L"category" == m_strValue)	return 1;
			if (L"categoryEl" == m_strValue)return 2;
			if (L"series" == m_strValue)	return 3;
			if (L"seriesEl" == m_strValue)	return 4;

			return 0;
		}
		void ChartBuildType::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0:		m_strValue = L"allAtOnce";			break;
			case 1:		m_strValue = L"category";		break;
			case 2:		m_strValue = L"categoryEl";		break;
			case 3:		m_strValue = L"series";	break;
			case 4:		m_strValue = L"seriesEl";		break;
			default:	m_strValue = L"allAtOnce";
			}
		}
	} // namespace Limit
} // namespace PPTX

