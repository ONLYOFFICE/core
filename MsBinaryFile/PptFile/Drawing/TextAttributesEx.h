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
#pragma once

#if !defined(_WIN32) && !defined (_WIN64)
#include "../../../DesktopEditor/graphics/aggplustypes.h"
#else
#include <windows.h>
#endif

#include "Structures.h"
#include "TextStructures.h"

namespace PPT
{
class CTheme;
class CLayout;

class CTextAttributesEx
{
public:
    LONG	m_lTextMasterType; // only ppt property
    _UINT32	m_lTextType;

    int		m_lStyleThemeIndex;

    int		m_lPlaceholderType;
    int		m_lPlaceholderID;


    // bounds
    Aggplus::RECT m_oBounds;

    // default settings
    ODRAW::CTextAttributes			m_oAttributes;
    std::vector<CParagraph>	m_arParagraphs;
    std::wstring m_originalText;

    bool	m_bVertical;
    bool	m_bAutoFit;

    int 	m_nTextFlow;


    int		m_lWrapMode; // 0 - square, default; 1 - none wrap


    // for ppt. to not change anything now
    CTextRuler				m_oRuler;

    CTextStyles				m_oLayoutStyles;
    CTextStyles				m_oStyles;

    // from pptx
    bool					m_bIsSlideFontRef;
    int						m_lFontRef;

    CTextAttributesEx();
    CTextAttributesEx& operator =(const CTextAttributesEx& oSrc);

    CTextAttributesEx(const CTextAttributesEx& oSrc);

    ~CTextAttributesEx();

    void NormalizeString(std::wstring& strText);

    void RecalcParagraphsPPT();
    void ApplyThemeStyle	(CTheme* pTheme = NULL);

    void ApplyRuler			(CTheme* pTheme);
    void ApplyRuler			(CTextPFRun* pPar, WORD lIndentLevel);
    bool IsEmptyText();
};
}
