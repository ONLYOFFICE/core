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

#include "CElement.h"

#include <iostream>
#include <algorithm>

CElement::CElement(std::wstring sNameElement, std::wstring sValue, std::wstring sNamespace, bool bNamespaceArguments)
    : m_sValue(sValue),
      m_sNameElement(sNameElement),
      m_sNamespace(sNamespace),
      m_bNamespaceArguments(bNamespaceArguments)
{
    m_bEmpty = true;
}

CElement::~CElement()
{
    Clear();
}

bool CElement::IsEmpty()
{
    return m_bEmpty;
}

int CElement::GetCountChildrens()
{
    return (int)m_arChildren.size();
}

int CElement::GetCountArguments()
{
    return (int)m_arArguments.size();
}

CElement* CElement::GetChildren(const int nIndex)
{
    if (nIndex < 0 || nIndex > GetCountChildrens())
        return NULL;
    return m_arChildren[nIndex];
}

std::pair<std::wstring, std::wstring>* CElement::GetArgument(const int nIndex)
{
    if (nIndex < 0 || nIndex > GetCountChildrens())
        return NULL;
    return &m_arArguments[nIndex];
}

std::wstring CElement::GetElementXML()
{
    std::wstring sText = L"<";

    if (m_sNamespace != L"")
        sText += m_sNamespace + L":";

    sText += m_sNameElement;

    sText += GetArguments();
    if (m_arClosingTag[m_sNameElement] == false)
        return (sText + L"/>");

    sText += L">";

    sText += m_sValue;

    for (int i = 0; i <(int)m_arChildren.size(); i++)
        sText += m_arChildren[i]->GetElementXML();

    sText += L"</";
    if (m_sNamespace != L"")
        sText += m_sNamespace + L":";

    sText += m_sNameElement + L">";
    return sText;
}

std::wstring CElement::GetArguments()
{
    if (m_arArguments.size() <= 0)
        return L"";

    std::wstring sText;

    for (int i = 0; i < (int)m_arArguments.size(); i++)
    {
        sText += L" ";

        if (m_sNamespace != L"" &&
            std::find(m_arNamespaceBan.begin(), m_arNamespaceBan.end(), m_sNamespace) == std::end(m_arNamespaceBan) &&
            m_bNamespaceArguments)
            sText += m_sNamespace + L":";

        sText += m_arArguments[i].first + L"=\"" + m_arArguments[i].second + L"\"";
    }
    return sText;
}

void CElement::AddArgument(std::wstring sNameArgument, std::wstring sValueArgument)
{
    m_bEmpty = false;

    m_arArguments.push_back(std::make_pair(sNameArgument, sValueArgument));
}

void CElement::DeleteChildren(CElement *oChildren)
{
    if (oChildren == nullptr)
        return;
    m_arChildren.erase(std::remove(m_arChildren.begin(), m_arChildren.end(), oChildren), m_arChildren.end());
}

void CElement::DeleteChildren(std::wstring sNameChildren)
{
    for (int i = 0; i < GetCountChildrens(); i++)
    {
        if (m_arChildren[i]->GetName() == sNameChildren)
        {
            DeleteChildren(m_arChildren[i]);
            return;
        }
    }
}

void CElement::DeleteArgument(std::wstring sNameArgument)
{
    for (auto iter = m_arArguments.begin(); iter != m_arArguments.end(); iter++)
        if (iter->first == sNameArgument)
        {
            m_arArguments.erase(iter);
            return;
        }
}

void CElement::Clear(bool Children, bool NameElement, bool Value, bool Arguments)
{
    if (Children)
    {
        for (CElement* element : m_arChildren)
            delete element;
        m_arChildren.clear();
    }
    if (NameElement)
        m_sNameElement = L"";
    if (Value)
        m_sValue = L"";
    if (Arguments)
        m_arArguments.clear();

    m_bEmpty = true;
}

void CElement::EditBoolNamespace(bool bNamespace)
{
    m_bNamespaceArguments = bNamespace;
}

void CElement::AddChildren(CElement* oChildren, int nIndex)
{
    m_bEmpty = false;

    if (nIndex < 0 || nIndex > (int)m_arChildren.size())
        m_arChildren.push_back(oChildren);
    else
        m_arChildren.insert(m_arChildren.begin() + nIndex, oChildren);
}

void CElement::setValue(std::wstring sValue)
{
    m_sValue = sValue;
    m_bEmpty = false;
}

void CElement::SetDefoult()
{
}

std::wstring CElement::GetName()
{
    return m_sNameElement;
}

std::wstring CElement::GetValue()
{
    return  m_sValue;
}
