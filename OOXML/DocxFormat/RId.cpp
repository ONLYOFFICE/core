/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
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
		//Учитывает только rid начинающиеся с rId, остальные сохраняем так как есть
		//Tогда не будет проблем с добавление новый id, мы всегда будем генерировать их с префиксом rId
		std::wstring sFindString(_T("rId"));
		size_t nFindStringLength = sFindString.length();

		if(0 == rid.find(sFindString) && rid.length() > nFindStringLength && 0 != isdigit(rid[nFindStringLength]))
		{
			std::wstring strParam = rid.substr(nFindStringLength);
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
