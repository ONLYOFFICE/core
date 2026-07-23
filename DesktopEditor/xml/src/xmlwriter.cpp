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
#include "../include/xmlwriter.h"
#include "../../common/File.h"

namespace XmlUtils
{
	class CXmlWriter::Impl
	{
	public:
		NSStringUtils::CStringBuilder m_str;
	};

	CXmlWriter::CXmlWriter() : impl_(NULL)
	{
		impl_ = std::make_shared<CXmlWriter::Impl>();
	}
	CXmlWriter::~CXmlWriter()
	{
	}

	std::wstring CXmlWriter::GetXmlString()
	{
		if (impl_) return impl_->m_str.GetData();
		else return L"";
	}
	void CXmlWriter::SetXmlString(const std::wstring& strValue)
	{
		if (impl_)
		{
			impl_->m_str.Clear();
			impl_->m_str.WriteString(strValue);
		}
	}

	void CXmlWriter::WriteEncodeXmlString(const std::string& sString)
	{
		if (impl_) impl_->m_str.WriteEncodeXmlString(sString);
	}
	bool CXmlWriter::SaveToFile(const std::wstring& strFilePath/*, bool bEncodingToUTF8 = false*/)
	{
		if (impl_)
			return NSFile::CFileBinary::SaveToFile(strFilePath, impl_->m_str.GetData());
		else
			return false;
	}
	void CXmlWriter::WriteString(const std::wstring& strValue)
	{
		if (impl_) impl_->m_str += strValue;
	}
	void CXmlWriter::WriteInteger(int Value)
	{
		if (impl_) impl_->m_str += std::to_wstring(Value);
	}
	void CXmlWriter::WriteDouble(double Value)
	{
		if (impl_) impl_->m_str += std::to_wstring(Value);
	}
	void CXmlWriter::WriteBoolean(bool Value)
	{
		if (!impl_) return;

		if (Value)
			impl_->m_str += (L"true");
		else
			impl_->m_str += (L"false");
	}
	void CXmlWriter::WriteNodeBegin(const std::wstring& strNodeName, bool bAttributed)
	{
		if (!impl_) return;

		impl_->m_str += (L"<") + strNodeName;

		if (!bAttributed)
			impl_->m_str += (L">");
	}
	void CXmlWriter::WriteNodeEnd(const std::wstring& strNodeName, bool bEmptyNode, bool bEndNode)
	{
		if (!impl_) return;

		if (bEmptyNode)
		{
			if (bEndNode)
				impl_->m_str += (L" />");
			else
				impl_->m_str += (L">");
		}
		else
			impl_->m_str += (L"</") + strNodeName + (L">");
	}
	void CXmlWriter::WriteNode(const std::wstring& strNodeName, const std::wstring& strNodeValue)
	{
		if (!impl_) return;

		if (strNodeValue.empty())
			impl_->m_str += L"<" + strNodeName + L"/>";
		else
			impl_->m_str += L"<" + strNodeName + L">" + strNodeValue + L"</" + strNodeName + L">";
	}
	void CXmlWriter::WriteNode(const std::wstring& strNodeName, int nValue, const std::wstring& strTextBeforeValue, const std::wstring& strTextAfterValue)
	{
		WriteNodeBegin(strNodeName);
		WriteString(strTextBeforeValue);
		WriteInteger(nValue);
		WriteString(strTextAfterValue);
		WriteNodeEnd(strNodeName);
	}
	void CXmlWriter::WriteNode(const std::wstring& strNodeName, double dValue)
	{
		WriteNodeBegin(strNodeName);
		WriteDouble(dValue);
		WriteNodeEnd(strNodeName);
	}
	void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const std::wstring& strAttributeValue)
	{
		if (impl_) impl_->m_str += L" " + strAttributeName + L"=\"" + strAttributeValue + L"\"";
	}
	void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, const std::string& strAttributeValue)
	{
		if (!impl_) return;

		impl_->m_str += L" " + strAttributeName + L"=\"";
		impl_->m_str.WriteEncodeXmlString(strAttributeValue);
		impl_->m_str += L"\"";
	}
	void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, int nValue, const std::wstring& strTextBeforeValue, const std::wstring& strTextAfterValue)
	{
		WriteString(L" " + strAttributeName + L"=");
		WriteString(L"\"");
		WriteString(strTextBeforeValue);
		WriteInteger(nValue);
		WriteString(strTextAfterValue);
		WriteString(L"\"");
	}
	void CXmlWriter::WriteAttribute(const std::wstring& strAttributeName, double dValue)
	{
		WriteString(L" " + strAttributeName + L"=");
		WriteString(L"\"");
		WriteDouble(dValue);
		WriteString(L"\"");
	}
}
