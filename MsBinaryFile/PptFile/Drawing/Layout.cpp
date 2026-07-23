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
#include "Layout.h"


void PPT::CorrectPlaceholderType(int& type)
{
    switch (type)
    {
    case PT_MasterTitle:			type = PT_Title;			break;
    case PT_MasterBody:				type = PT_Body;				break;
    case PT_MasterCenterTitle:		type = PT_CenterTitle;		break;
    case PT_MasterSubTitle:			type = PT_SubTitle;			break;
    case PT_MasterNotesSlideImage:	type = PT_NotesSlideImage;	break;
    case PT_MasterNotesBody:		type = PT_NotesBody;		break;
    }
}

bool PPT::isTitlePlaceholder(int type)
{
    switch (type)
    {
    case PT_MasterTitle:
    case PT_MasterCenterTitle:
    case PT_Title:
    case PT_CenterTitle:
    case PT_VerticalTitle:
        return true;
    default:
        return false;
    }
}

bool PPT::isBodyPlaceholder(int type)
{
    switch (type)
    {
    case PT_MasterBody:
    case PT_Body:
    case PT_VerticalBody:
        return true;
    default:
        return false;
    }
}

PPT::CLayout::CLayout()
{
    Clear();
}

void PPT::CLayout::Clear()
{
    m_arElements.clear();
    m_mapPlaceholders.clear();

    m_bIsTitleMaster = false;

    m_bHasDate = false;
    m_bHasSlideNumber = false;
    m_bHasFooter = false;
    m_nFormatDate = 1;

    for (int i = 0; i < 3; i++)
        m_PlaceholdersReplaceString[i].clear();

    m_bUseThemeColorScheme = true;
    m_bShowMasterShapes = true;
    m_strLayoutType = L"obj";
    m_bIsBackground = false;
}

void PPT::CLayout::CreateDublicateElements()
{
    // simply make duplicates of all our elements

    for (size_t nIndex = 0; nIndex < m_arElements.size(); ++nIndex)
    {
        CElementPtr pElem = m_arElements[nIndex];
        if (NULL != pElem)
        {
            m_arElements[nIndex] = pElem->CreateDublicate();
        }
    }
}

PPT::CElementPtr PPT::CLayout::GetPlaceholder(LONG lID)
{
    size_t nCount = m_arElements.size();

    for (size_t i = 0; i < nCount; ++i)
    {
        CElementPtr pElem = m_arElements[i];

        if (pElem->m_lPlaceholderType == lID)
        {
            return pElem;
        }
    }

    return CElementPtr();
}

LONG PPT::CLayout::GetCountPlaceholderWithType(LONG lType)
{
    LONG lFound = 0;

    for (size_t i = 0; i < m_arElements.size(); ++i)
    {
        if (m_arElements[i]->m_lPlaceholderType == lType)
        {
            ++lFound;
        }
    }

    return lFound;
}

ODRAW::CColor PPT::CLayout::GetColor(const LONG& lIndexScheme)
{
    if (lIndexScheme < (LONG)m_arColorScheme.size())
    {
        return m_arColorScheme[lIndexScheme];
    }
    return ODRAW::CColor();
}

void PPT::CLayout::CheckPlaceholderStyle(std::wstring strStyleName, LONG& lType, LONG& lLevel, LONG& lTypeStyle)
{
    size_t		nLen = strStyleName.length();
    wchar_t* pData = (wchar_t*)strStyleName.c_str();

    lType = 0;
    lLevel = 0;
    lTypeStyle = -1;

    wchar_t* pDataEnd = pData + nLen;
    wchar_t* pDataMem = pData + 1;
    for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
    {
        lType *= 10;
        lType += (*pDataMem - ((wchar_t)'0'));
    }

    pDataMem += 4;
    for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
    {
        lLevel *= 10;
        lLevel += (*pDataMem - ((wchar_t)'0'));
    }

    ++pDataMem;
    if (pDataMem + 1 < pDataEnd)
    {
        if ((wchar_t('c') == pDataMem[0]) && (wchar_t('f') == pDataMem[1]))
            lTypeStyle = 1;
        if ((wchar_t('p') == pDataMem[0]) && (wchar_t('f') == pDataMem[1]))
            lTypeStyle = 0;
    }
}
