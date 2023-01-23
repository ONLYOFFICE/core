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
