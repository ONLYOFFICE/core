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

#include "BevelType.h"

namespace PPTX
{
	namespace Limit
	{		
		BevelType::BevelType()
		{
			m_strValue = L"circle";
		}
		void BevelType::set(const std::wstring& strValue)
		{
			m_strValue = strValue;
		}
		unsigned char BevelType::GetBYTECode() const
		{
			if (L"angle" == m_strValue)			return 0;
			if (L"artDeco" == m_strValue)		return 1;
			if (L"circle" == m_strValue)		return 2;
			if (L"convex" == m_strValue)		return 3;
			if (L"coolSlant" == m_strValue)		return 4;
			if (L"cross" == m_strValue)			return 5;
			if (L"divot" == m_strValue)			return 6;
			if (L"hardEdge" == m_strValue)		return 7;
			if (L"relaxedInset" == m_strValue)	return 8;
			if (L"riblet" == m_strValue)		return 9;
			if (L"slope" == m_strValue)			return 10;
			if (L"softRound" == m_strValue)		return 11;
			return 0;
		}
		void BevelType::SetBYTECode(const unsigned char& val)
		{
			switch (val)
			{
				case 0:		m_strValue = L"angle";		break;
				case 1:		m_strValue = L"artDeco";	break;
				case 2:		m_strValue = L"circle";		break;
				case 3:		m_strValue = L"convex";		break;
				case 4:		m_strValue = L"coolSlant";	break;
				case 5:		m_strValue = L"cross";		break;
				case 6:		m_strValue = L"divot";		break;
				case 7:		m_strValue = L"hardEdge";	break;
				case 8:		m_strValue = L"relaxedInset";break;
				case 9:		m_strValue = L"riblet";		break;
				case 10:	m_strValue = L"slope";		break;
				case 11:	m_strValue = L"softRound";	break;
				default:	m_strValue = L"angle";
			}
		}
	} // namespace Limit
} // namespace PPTX
