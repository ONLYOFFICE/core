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

#include "RId.h"

namespace OOX
{
	RId::RId() : m_id(0)
	{
		bNumber = false;
	}
	RId::RId(const unsigned int id) : m_id(id)
	{
		bNumber = true;
	}
	RId::RId(const std::wstring& rid)
	{
		(*this) = rid;
	}
	RId::RId(const RId& oSrc)
	{
		(*this) = oSrc;
	}
	const RId& RId::operator= (const std::wstring& rid)
	{
		//Only process rids that start with rId, others are saved as-is
		//Then there will be no problems with adding new ids, we always generate them with the rId prefix
		std::wstring sFindString(L"rId");

		if (0 == rid.find(sFindString) && rid.length() > 3 && rid.length() < 11 && 0 != isdigit(rid[3]))
		{
			std::wstring strParam = rid.substr(3);
			m_id = XmlUtils::GetUInteger(strParam);
			bNumber = true;
		}
		else
		{
			m_id = 0;
			m_sId = rid;
			bNumber = false;
		}

		return *this;
	}
	const RId& RId::operator= (const RId& oSrc)
	{
		m_id	= oSrc.m_id;
		m_sId	= oSrc.m_sId;
		bNumber = oSrc.bNumber;
		return *this;
	}
	const bool RId::operator ==(const RId& lhs) const
	{
		return m_id == lhs.m_id && m_sId == lhs.m_sId;
	}
	const bool RId::operator !=(const RId& lhs) const
	{
		return !operator ==(lhs);
	}
	const bool RId::operator < (const RId& lhs) const
	{
		return m_id < lhs.m_id;
	}
	const bool RId::operator <=(const RId& lhs) const
	{
		return m_id <= lhs.m_id;
	}
	const bool RId::operator >(const RId& lhs) const
	{
		return m_id > lhs.m_id;
	}
	const bool RId::operator >=(const RId& lhs) const
	{
		return m_id >= lhs.m_id;
	}

	std::wstring RId::get() const
	{
		return ToString();
	}
	unsigned int RId::getNumber() const
	{
		return m_id;
	}

	const RId  RId::next() const
	{
		bNumber	= true;
		return RId(m_id + 1);
	}
	const std::wstring RId::ToString() const
		{
			if(!m_sId.empty())
				return m_sId;
			else if (bNumber)
				return _T("rId") + std::to_wstring((unsigned int)m_id);
			else
				return L"";
		}

} // namespace OOX
