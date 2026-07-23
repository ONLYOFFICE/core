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

#include "PlaceholderType.h"

namespace PPTX
{
	namespace Limit
	{
		int GetPhType(const std::wstring& strType)
		{
			if (L"body" == strType)
				return 0;
			if (L"chart" == strType)
				return 1;
			if (L"clipArt" == strType)
				return 2;
			if (L"ctrTitle" == strType)
				return 15;
			if (L"dgm" == strType)
				return 4;
			if (L"dt" == strType)
				return 5;
			if (L"ftr" == strType)
				return 6;
			if (L"hdr" == strType)
				return 7;
			if (L"media" == strType)
				return 8;
			if (L"obj" == strType)
				return 9;
			if (L"pic" == strType)
				return 10;
			if (L"sldImg" == strType)
				return 11;
			if (L"sldNum" == strType)
				return 12;
			if (L"subTitle" == strType)
				return 0;
			if (L"tbl" == strType)
				return 14;
			if (L"title" == strType)
				return 15;
			return 0;
		}

		PlaceholderType::PlaceholderType()
		{
			m_strValue = L"obj";
		}
		void PlaceholderType::set(const std::wstring& strValue)
		{
			if ((L"body" == strValue) ||
				(L"chart" == strValue) ||
				(L"clipArt" == strValue) ||
				(L"ctrTitle" == strValue) ||
				(L"dgm" == strValue) ||
				(L"dt" == strValue) ||
				(L"ftr" == strValue) ||
				(L"hdr" == strValue) ||
				(L"media" == strValue) ||
				(L"obj" == strValue) ||
				(L"pic" == strValue) ||
				(L"sldImg" == strValue) ||
				(L"sldNum" == strValue) ||
				(L"subTitle" == strValue) ||
				(L"tbl" == strValue) ||
				(L"title" == strValue))
			{
				m_strValue = strValue;
			}
		}
		unsigned char PlaceholderType::GetBYTECode() const
		{
			if (L"body" == m_strValue)
				return 0;
			if (L"chart" == m_strValue)
				return 1;
			if (L"clipArt" == m_strValue)
				return 2;
			if (L"ctrTitle" == m_strValue)
				return 3;
			if (L"dgm" == m_strValue)
				return 4;
			if (L"dt" == m_strValue)
				return 5;
			if (L"ftr" == m_strValue)
				return 6;
			if (L"hdr" == m_strValue)
				return 7;
			if (L"media" == m_strValue)
				return 8;
			if (L"obj" == m_strValue)
				return 9;
			if (L"pic" == m_strValue)
				return 10;
			if (L"sldImg" == m_strValue)
				return 11;
			if (L"sldNum" == m_strValue)
				return 12;
			if (L"subTitle" == m_strValue)
				return 13;
			if (L"tbl" == m_strValue)
				return 14;
			if (L"title" == m_strValue)
				return 15;
			return 9;
		}
		void PlaceholderType::SetBYTECode(const unsigned char& src)
		{
			switch (src)
			{
			case 0:
				m_strValue = L"body";
				break;
			case 1:
				m_strValue = L"chart";
				break;
			case 2:
				m_strValue = L"clipArt";
				break;
			case 3:
				m_strValue = L"ctrTitle";
				break;
			case 4:
				m_strValue = L"dgm";
				break;
			case 5:
				m_strValue = L"dt";
				break;
			case 6:
				m_strValue = L"ftr";
				break;
			case 7:
				m_strValue = L"hdr";
				break;
			case 8:
				m_strValue = L"media";
				break;
			case 9:
				m_strValue = L"obj";
				break;
			case 10:
				m_strValue = L"pic";
				break;
			case 11:
				m_strValue = L"sldImg";
				break;
			case 12:
				m_strValue = L"sldNum";
				break;
			case 13:
				m_strValue = L"subTitle";
				break;
			case 14:
				m_strValue = L"tbl";
				break;
			case 15:
				m_strValue = L"title";
				break;
			default:
				m_strValue = L"obj";
				break;
			}
		}
	} // namespace Limit
} // namespace PPTX
