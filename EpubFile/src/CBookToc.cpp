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

#include "CBookToc.h"

CBookToc::CBookToc()
{
}

CBookToc::~CBookToc()
{
}

bool CBookToc::ReadToc(XmlUtils::CXmlLiteReader& oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return false;

    int nParentDepth = oXmlLiteReader.GetDepth();
    while(oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = oXmlLiteReader.GetName();
        if (sName == L"docTitle")
            ReadTitle(oXmlLiteReader);
        else if(sName == L"navMap")
            ReadMap(oXmlLiteReader);
    }

    return true;
}

void CBookToc::ReadTitle(XmlUtils::CXmlLiteReader& oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return;

    oXmlLiteReader.ReadNextNode();
    m_sTitle = oXmlLiteReader.GetText2();
}

std::wstring CBookToc::GetAttributeValue(XmlUtils::CXmlLiteReader& oXmlLiteReader, const std::wstring& sAttributeName) const
{
    while(oXmlLiteReader.MoveToNextAttribute())
    {
        if (oXmlLiteReader.GetName() == sAttributeName)
        {
            std::wstring sText = oXmlLiteReader.GetText();
            size_t posLastSlash = sText.rfind(L'/');
            if (posLastSlash != std::wstring::npos)
                sText.erase(0, posLastSlash + 1);
            oXmlLiteReader.MoveToElement();
            return sText;
        }
    }
    oXmlLiteReader.MoveToElement();
    return L"";
}

void CBookToc::ReadMap(XmlUtils::CXmlLiteReader& oXmlLiteReader)
{
    if (!oXmlLiteReader.IsValid() || oXmlLiteReader.IsEmptyNode())
        return;

    int nParentDepth = oXmlLiteReader.GetDepth();
    m_structData structData;
    while (oXmlLiteReader.ReadNextSiblingNode(nParentDepth))
    {
        std::wstring sName = oXmlLiteReader.GetName();
        if (sName == L"navPoint")
        {
            structData.sID = GetAttributeValue(oXmlLiteReader, L"id");
            structData.sPlayOrder = GetAttributeValue(oXmlLiteReader, L"playOrder");
        }
        else if (sName == L"navLabel")
        {
            oXmlLiteReader.ReadNextNode();
            structData.sText = oXmlLiteReader.GetText2();
        }
        else if (sName == L"content")
        {
            structData.sRef = GetAttributeValue(oXmlLiteReader, L"src");
        }

        if (structData.Full())
        {
            m_arData.push_back(structData);
            structData.Clear();
        }
        ReadMap(oXmlLiteReader);
    }
}

/*
void CBookToc::ShowToc() const
{
    std::wcout << L"-----TOC-----" << std::endl;
    std::wcout << "Title - " << m_sTitle << std::endl;
    for (const m_structData& oData : m_arData)
    {
        std::wcout << oData.sPlayOrder << " - " << oData.sID << " - " << oData.sText << " - " << oData.sRef << std::endl;
//        std::wstring sl = m_arData[i].sText + L"\n";
//        std::fputws(sl.c_str() , fl);
    }
}

int CBookToc::GetCountToc() const
{
    return m_arData.size();
}

const std::pair<std::wstring, std::wstring> CBookToc::GetTextAndRef(int nIndex) const
{
    return std::make_pair(nIndex < 0 || nIndex > GetCountToc() ? L"", L"" : m_arData[nIndex].sText, m_arData[nIndex].sRef);
}
*/

void CBookToc::Clear()
{
    m_sTitle.clear();
    m_arData.clear();
}

