/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#include "../include/xmlutils.h"
#include "./xmllight_private.h"

namespace XmlUtils
{
	CXmlLiteReader::CXmlLiteReader()
	{
		m_pInternal = new CXmlLiteReader_Private();
	}
	CXmlLiteReader::~CXmlLiteReader()
	{
		delete m_pInternal;
	}

	void CXmlLiteReader::Clear()
	{
		m_pInternal->Clear();
	}
	bool CXmlLiteReader::IsValid()
	{
		return m_pInternal->IsValid();
	}

	bool CXmlLiteReader::FromFile(const wchar_t* sFilePath)
	{
		return m_pInternal->FromFile(sFilePath);
	}
	bool CXmlLiteReader::FromFile(const std::wstring& sFilePath)
	{
		return m_pInternal->FromFile(sFilePath);
	}
	bool CXmlLiteReader::FromString(const wchar_t* sXml)
	{
		return m_pInternal->FromString(sXml);
	}
	bool CXmlLiteReader::FromString(const std::wstring& sXml)
	{
		return m_pInternal->FromString(sXml);
	}
	bool CXmlLiteReader::FromStringA(const std::string& sXml)
	{
		return m_pInternal->FromStringA(sXml);
	}

	bool CXmlLiteReader::Read(XmlNodeType &oNodeType)
	{
		return m_pInternal->Read(oNodeType);
	}
	bool CXmlLiteReader::ReadNextNode()
	{
		return m_pInternal->ReadNextNode();
	}
	bool CXmlLiteReader::ReadNextSiblingNode(int nDepth)
	{
		return m_pInternal->ReadNextSiblingNode(nDepth);
	}
	bool CXmlLiteReader::ReadNextSiblingNode2(int nDepth)
	{
		return m_pInternal->ReadNextSiblingNode2(nDepth);
	}
	bool CXmlLiteReader::ReadTillEnd(int nDepth)
	{
		return m_pInternal->ReadTillEnd(nDepth);
	}

	std::wstring CXmlLiteReader::GetName()
	{
		return m_pInternal->GetName();
	}
	std::string CXmlLiteReader::GetNameA()
	{
		return m_pInternal->GetNameA();
	}
	int CXmlLiteReader::GetDepth()
	{
		return m_pInternal->GetDepth();
	}
	bool CXmlLiteReader::IsEmptyNode()
	{
		return m_pInternal->IsEmptyNode();
	}

	std::wstring CXmlLiteReader::GetText()
	{
		return m_pInternal->GetText();
	}
	std::string CXmlLiteReader::GetTextA()
	{
		return m_pInternal->GetTextA();
	}

	std::wstring CXmlLiteReader::GetText2()
	{
		return m_pInternal->GetText2();
	}
	std::string CXmlLiteReader::GetText2A()
	{
		return m_pInternal->GetText2A();
	}

	std::wstring CXmlLiteReader::GetOuterXml()
	{
		return m_pInternal->GetOuterXml();
	}
	std::wstring CXmlLiteReader::GetInnerXml()
	{
		return m_pInternal->GetInnerXml();
	}

	int CXmlLiteReader::GetAttributesCount()
	{
		return m_pInternal->GetAttributesCount();
	}
	bool CXmlLiteReader::MoveToFirstAttribute()
	{
		return m_pInternal->MoveToFirstAttribute();
	}
	bool CXmlLiteReader::MoveToNextAttribute()
	{
		return m_pInternal->MoveToNextAttribute();
	}
	bool CXmlLiteReader::MoveToElement()
	{
		return m_pInternal->MoveToElement();
	}

	bool CXmlLiteReader::IsEmptyElement()
	{
		return m_pInternal->IsEmptyElement();
	}
}