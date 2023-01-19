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


    // граница
    Aggplus::RECT m_oBounds;

    // настройки по-умолчанию
    ODRAW::CTextAttributes			m_oAttributes;
    std::vector<CParagraph>	m_arParagraphs;
    std::wstring m_originalText;

    bool	m_bVertical;
    bool	m_bAutoFit;

    int 	m_nTextFlow;


    int		m_lWrapMode; // 0 - square, default; 1 - none wrap


    // для ппт. чтобы не менять счас ничего
    CTextRuler				m_oRuler;

    CTextStyles				m_oLayoutStyles;
    CTextStyles				m_oStyles;

    // из пптх
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
