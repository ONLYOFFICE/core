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
#include "TextFullSettings.h"

using namespace PPT;

CTextFullSettings::CTextFullSettings() : m_arRanges()
{
    m_pTextStyleProp	= NULL;
    m_pTextSpecInfo		= NULL;
    m_pTextRuler		= NULL;

    m_nTextType			= 0xFFFFFFFF;
    m_lStyleThemeIndex	= 0; //default
    m_strText.clear();
}

CTextFullSettings::CTextFullSettings(const CTextFullSettings &oSrc)
{
    *this = oSrc;
}

CTextFullSettings &CTextFullSettings::operator =(const CTextFullSettings &oSrc)
{
    m_pTextStyleProp	= oSrc.m_pTextStyleProp;
    m_pTextSpecInfo		= oSrc.m_pTextSpecInfo;
    m_pTextRuler		= oSrc.m_pTextRuler;
    m_lStyleThemeIndex	= oSrc.m_lStyleThemeIndex;

    m_nTextType			= oSrc.m_nTextType;
    m_strText			= oSrc.m_strText;

    m_arRanges = oSrc.m_arRanges;
    return (*this);
}

CTextFullSettings::~CTextFullSettings()
{
    m_pTextStyleProp = NULL;
    m_pTextSpecInfo = NULL;
    m_pTextRuler = NULL;
}

std::wstring CTextFullSettings::ApplyProperties(CTextAttributesEx *pTextAttributes)
{
    if (NULL == pTextAttributes)
        return m_strText;

    pTextAttributes->m_lTextType		= m_nTextType;
    pTextAttributes->m_lStyleThemeIndex	= m_lStyleThemeIndex;

    if (NULL != m_pTextStyleProp)
        ConvertPPTTextToEditorStructure(m_pTextStyleProp->m_arrPFs, m_pTextStyleProp->m_arrCFs, m_strText, *pTextAttributes);

    if (NULL != m_pTextRuler)
        pTextAttributes->m_oRuler = m_pTextRuler->m_oTextRuler;

    return m_strText;
}
