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
#include "../include/xmlutils.h"
#include "./xmllight_private.h"

namespace XmlUtils
{
    static void libxml2_err_no(void * ctx, const char * msg, ...)
    {
        // none
    }

    IXmlDOMDocument::IXmlDOMDocument()
    {
        m_lRef = 1;
    }
    IXmlDOMDocument::~IXmlDOMDocument()
    {
    }

    unsigned int IXmlDOMDocument::AddRef()
    {
        ++m_lRef;
        return m_lRef;
    }
    unsigned int IXmlDOMDocument::Release()
    {
        unsigned int lReturn = --m_lRef;
        if (0 == m_lRef)
            delete this;
        return lReturn;
    }

    void IXmlDOMDocument::DisableOutput()
    {
        xmlSetGenericErrorFunc(NULL, (xmlGenericErrorFunc)libxml2_err_no);
    }

    void IXmlDOMDocument::EnableOutput()
    {
        xmlSetGenericErrorFunc(NULL, NULL);
    }
}

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
    bool CXmlLiteReader::FromStringA(const char* sXml, int size)
    {
        return m_pInternal->FromStringA(sXml, size);
    }
    bool CXmlLiteReader::MoveToStart()
    {
        return m_pInternal->MoveToStart();
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
    std::wstring CXmlLiteReader::GetNameNoNS()
    {
        return m_pInternal->GetNameNoNS();
    }
    std::string CXmlLiteReader::GetNameNoNSA()
    {
        return m_pInternal->GetNameNoNSA();
    }
	const char* CXmlLiteReader::GetNameChar()
	{
		return m_pInternal->GetNameChar();
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
	const char* CXmlLiteReader::GetTextChar()
	{
		return m_pInternal->GetTextChar();
	}
	std::wstring CXmlLiteReader::GetAttributeTextWithHHHH()
	{
		return m_pInternal->GetAttributeTextWithHHHH();
	}

	std::wstring CXmlLiteReader::GetText2()
	{
		return m_pInternal->GetText2();
	}
	std::string CXmlLiteReader::GetText2A()
	{
		return m_pInternal->GetText2A();
	}
    std::wstring CXmlLiteReader::GetText3()
	{
		return m_pInternal->GetText3();
	}
	void CXmlLiteReader::GetTextWithHHHH(bool bPreserve, wchar_t*& sBuffer, long& nSize, long& nLen)
	{
		return m_pInternal->GetTextWithHHHH(bPreserve, sBuffer, nSize,nLen);
	}
	std::wstring CXmlLiteReader::GetTextWithHHHH(bool bPreserve)
	{
		return m_pInternal->GetTextWithHHHH(bPreserve);
	}

	std::wstring CXmlLiteReader::GetOuterXml(bool bEncodingXml)
	{
		return m_pInternal->GetOuterXml(bEncodingXml);
	}
	std::wstring CXmlLiteReader::GetInnerXml(bool bEncodingXml)
	{
		return m_pInternal->GetInnerXml(bEncodingXml);
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

    std::wstring CXmlLiteReader::GetNamespacePrefix()
    {
        return m_pInternal->GetNamespacePrefix();
    }
    XmlNodeType CXmlLiteReader::GetNodeType()
    {
        return m_pInternal->GetNodeType();
    }
    bool CXmlLiteReader::IsDefaultAttribute()
    {
        return m_pInternal->IsDefaultAttribute();
    }
}
