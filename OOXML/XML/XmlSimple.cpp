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
#include "../Base/Nullable.h"
#include "../../DesktopEditor/common/File.h"
#include "XmlSimple.h"

namespace XmlUtils
{
	CAttribute::CAttribute()
	{
		m_strValue = L"";
	}
	void CAttribute::Write(const std::wstring& strName, const int& value)
	{
		m_strValue += L" " + strName + L"=\"" + std::to_wstring(value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const size_t& value)
	{
		m_strValue += L" " + strName + L"=\"" + std::to_wstring((unsigned int)value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const double& value)
	{
		m_strValue += L" " + strName + L"=\"" + XmlUtils::ToString(value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const std::wstring& value)
	{
		m_strValue += (L" ") + strName + L"=\"" + value + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const bool& value)
	{
		if (value)
			m_strValue += (L" ") + strName + L"=\"true\"";
		else
			m_strValue += (L" ") + strName + L"=\"false\"";
	}

	void CAttribute::Write(const std::wstring& strName, const nullable_int& value)
	{
		if (!value.IsInit())
			return;
		m_strValue += L" " + strName + L"=\"" + std::to_wstring(*value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const nullable_uint& value)
	{
		if (!value.IsInit())
			return;
		m_strValue += L" " + strName + L"=\"" + std::to_wstring(*value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const nullable_sizet& value)
	{
		if (!value.IsInit())
			return;
		m_strValue += L" " + strName + L"=\"" + std::to_wstring((unsigned int)*value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const nullable_double& value)
	{
		if (!value.IsInit())
			return;
		m_strValue += L" " + strName + L"=\"" + XmlUtils::ToString(*value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const nullable_string& value)
	{
		if (!value.IsInit())
			return;
		m_strValue += (L" ") + strName + L"=\"" + *value + L"\"";
	}
	void CAttribute::Write2(const std::wstring& strName, const nullable_string& value)
	{
		if (!value.IsInit())
			return;
		m_strValue += (L" ") + strName + L"=\"" + XmlUtils::EncodeXmlString(*value) + L"\"";
	}
	void CAttribute::Write(const std::wstring& strName, const nullable_bool& value)
	{
		if (!value.IsInit())
			return;
		
		m_strValue += (L" ") + strName + ((*value) ? L"=\"1\"" : L"=\"0\"");
	}
	void CAttribute::Write2(const std::wstring& strName, const nullable_bool& value)
	{
		if (!value.IsInit())
			return;
		m_strValue += (L" ") + strName + ((*value) ? L"=\"true\"" : L"=\"false\"");
	}

	CAttribute::CAttribute(const CAttribute& oSrc)
	{
		*this = oSrc;
	}
	CAttribute& CAttribute::operator=(const CAttribute& oSrc)
	{
		m_strValue = oSrc.m_strValue;
		return (*this);
	}

	CNodeValue::CNodeValue()
	{
		m_strValue = L"";
	}
	
	// --------------------------------------------------------------- //
	void CNodeValue::Write2(const std::wstring& strName, const int& value)
	{
		Write2(strName, std::to_wstring(value));
	}
	void CNodeValue::Write2(const std::wstring& strName, const size_t& value)
	{
		std::wstring s = L"=\"" + std::to_wstring((unsigned int)value) + L"\"";
		Write2(strName, s);
	}
	void CNodeValue::Write2(const std::wstring& strName, const double& value)
	{
		std::wstring s = L"=\"" + XmlUtils::ToString(value) + L"\"";
		Write2(strName, s);
	}
	void CNodeValue::Write2(const std::wstring& strName, const std::wstring& value)
	{
		m_strValue += (L"<") + strName + L">" + value + L"</" + strName + L">";
	}
	void CNodeValue::Write2(const std::wstring& strName, const bool& value)
	{
		if (value)
			m_strValue += (L"<") + strName + L">true</" + strName + L">";
		else
			m_strValue += (L"<") + strName + L">false</" + strName + L">";
	}

	// ---------------------------------------------------------------- //
	void CNodeValue::Write2(const std::wstring& strName, const nullable_int& value)
	{
		if (value.IsInit())
			Write2(strName, *value);
	}
	void CNodeValue::Write2(const std::wstring& strName, const nullable_sizet& value)
	{
		if (value.IsInit())
			Write2(strName, *value);
	}
	void CNodeValue::Write2(const std::wstring& strName, const nullable_double& value)
	{
		if (value.IsInit())
			Write2(strName, *value);
	}
	void CNodeValue::Write2(const std::wstring& strName, const nullable_string& value)
	{
		if (value.IsInit())
			Write2(strName, *value);
	}
	void CNodeValue::Write2(const std::wstring& strName, const nullable_bool& value)
	{
		if (value.IsInit())
			Write2(strName, *value);
	}

	std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr)
	{
		if (strName.empty()) return L"";

		return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L"/>";
	}
	std::wstring CreateNode(const std::wstring& strName, const CNodeValue& oNode)
	{
		if (strName.empty()) return L"";

		if (oNode.m_strValue.empty())
			return L"<" + strName + L"/>";

		return L"<" + strName + L">" + oNode.m_strValue + L"</" + strName + L">";
	}
	std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr, const CNodeValue& oNode)
	{
		if (strName.empty()) return L"";

		if ( oNode.m_strValue.empty())
			return CreateNode(strName, oAttr);
		
		return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L">" + oNode.m_strValue +  L"</" + strName + L">";
	}
	std::wstring CreateNode(const std::wstring& strName, const CAttribute& oAttr, const std::wstring& strXml)
	{
		if (strName.empty()) return L"";

		if (strXml.empty())	return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L"/>";
		else				return L"<" + strName + (oAttr.m_strValue.empty() ? L"" : L" " + oAttr.m_strValue) + L">" + strXml + L"</" + strName + L">";
		
	}
	std::wstring CreateNode(const std::wstring& strName, const std::wstring& strXml)
	{
		if (strName.empty()) return L"";

		if (strXml.empty()) return L"<" + strName + L"/>";
		else				return L"<" + strName + L">" + strXml + L"</" + strName + L">";
	}

	void SaveToFile(const std::wstring& strFile, const std::wstring& strXml)
	{
		NSFile::CFileBinary::SaveToFile(strFile, strXml);
	}
}
