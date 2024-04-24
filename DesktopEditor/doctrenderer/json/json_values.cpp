/*
 * (c) Copyright Ascensio System SIA 2010-2024
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

#include "json_values.h"

namespace NSJSON
{
	IBaseValue::IBaseValue()
	{
	}

	IBaseValue::~IBaseValue()
	{
	}

	CPrimitive::CPrimitive(bool value) : m_type(ptBoolean)
	{
		m_bool = value;
	}

	CPrimitive::CPrimitive(int value) : m_type(ptInteger)
	{
		m_int = value;
	}

	CPrimitive::CPrimitive(double value) : m_type(ptDouble)
	{
		m_double = value;
	}

	CPrimitive::CPrimitive(const std::string& str) : m_type(ptStringA)
	{
		new (&m_string) std::string(str);
	}

	CPrimitive::CPrimitive(const std::wstring& wstr) : m_type(ptStringW)
	{
		new (&m_wstring) std::wstring(wstr);
	}

	CPrimitive::~CPrimitive()
	{
		switch (m_type)
		{
		case ptStringA:
			m_string.~basic_string<char>();
			break;
		case ptStringW:
			m_wstring.~basic_string<wchar_t>();
			break;
		default:
			break;
		}
	}

	bool CPrimitive::isBool() const
	{
		return m_type == ptBoolean;
	}

	bool CPrimitive::isInt() const
	{
		return m_type == ptInteger;
	}

	bool CPrimitive::isDouble() const
	{
		return m_type == ptDouble;
	}

	bool CPrimitive::isStringA() const
	{
		return m_type == ptStringA;
	}

	bool CPrimitive::isStringW() const
	{
		return m_type == ptStringW;
	}

	bool CPrimitive::toBool() const
	{
		if (m_type == ptBoolean)
			return m_bool;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return false;
	}

	int CPrimitive::toInt() const
	{
		if (m_type == ptInteger)
			return m_int;
		if (m_type == ptDouble)
			return (int)m_double;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return 0;
	}

	double CPrimitive::toDouble() const
	{
		if (m_type == ptDouble)
			return m_double;
		if (m_type == ptInteger)
			return (double)m_int;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return 0.0;
	}

	std::string CPrimitive::toStringA() const
	{
		if (m_type == ptStringA)
			return m_string;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return "";
	}

	std::wstring CPrimitive::toStringW() const
	{
		if (m_type == ptStringW)
			return m_wstring;
#ifdef JSON_DEBUG
		throw std::bad_cast();
#endif
		return L"";
	}

	CArray::CArray(int count) : m_values(count)
	{
	}

	CArray::CArray(std::initializer_list<CValue> elements) : m_values(elements)
	{
	}

	CArray::~CArray()
	{
	}

	int CArray::getCount() const
	{
		return (int)m_values.size();
	}

	CValue& CArray::get(int index)
	{
		return m_values[index];
	}

	CTypedArray::CTypedArray(BYTE* data, int len, bool isExternalize) : m_data(data), m_len(len), m_isExternalize(isExternalize)
	{
	}

	CTypedArray::~CTypedArray()
	{
		if (!m_isExternalize)
		{
			CValue::FreeTypedArray(m_data, m_len);
		}
	}

	BYTE* CTypedArray::getData()
	{
		return m_data;
	}

	int CTypedArray::getCount() const
	{
		return m_len;
	}

	CObject::CObject()
	{
	}

	CObject::~CObject()
	{
	}

	CValue& CObject::get(const std::string& name)
	{
		return m_values[name];
	}

	std::vector<std::string> CObject::getPropertyNames()
	{
		std::vector<std::string> ret;
		for (const storage_t::value_type& entry : m_values)
		{
			if (!entry.second.IsUndefined())
				ret.push_back(entry.first);
		}
		return ret;
	}
}
