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
#include "Structures.h"
#include "Layout.h"

namespace PPT
{
enum _typeMaster
{
    typeMaster,
    typeNotesMaster,
    typeHandoutMaster

};

const long g_ThemeTextStylesCount	= 4;

class CTheme
{
public:
    _typeMaster						m_eType;
    std::map<_UINT64, LONG>			m_mapGeomToLayout;// standard templates
    std::map<_UINT32, LONG>			m_mapTitleLayout; // header templates

    std::multimap<int,CElementPtr>	m_mapPlaceholders;

    std::vector<ODRAW::CColor>				m_arColorScheme;
    std::vector<CFontProperty>		m_arFonts;

    std::vector<ODRAW::CBrush>				m_arBrushes;
    std::vector<ODRAW::CPen>				m_arPens;

    CFontProperties					majorFont;
    CFontProperties					minorFont;

    std::vector<CEffects>			m_arEffects;
    CTextStyles						m_pStyles[g_ThemeTextStylesCount];
    std::vector<CLayoutPtr>			m_arLayouts;

    bool							m_bIsBackground;
    ODRAW::CBrush							m_oBackground;

    std::vector<CElementPtr>		m_arElements;

    std::wstring					m_sThemeName;

    std::vector<std::vector<ODRAW::CColor>>m_arExtraColorScheme;

    bool							m_bHasDate;
    bool							m_bHasSlideNumber;
    bool							m_bHasFooter;

    int								m_nFormatDate;//1- current, 2 - user

    vector_string					m_PlaceholdersReplaceString[3]; //0-dates, 1 - headers, 2 - footers

    std::vector<CBulletAutoNum>     m_arBullet;
    bool m_wasBulletStart;
    unsigned m_bulletPos;
    //------------------------------------------------------------------------------------
    CTheme(_typeMaster type = typeMaster);
    void Clear();

    virtual ~CTheme();
    ODRAW::CColor GetColor(const LONG& lIndexScheme);

    void CalculateStyles();

    static void CalculateStyle(CTheme* pTheme, CTextStyles& oStyle);
};
typedef boost::shared_ptr<CTheme> CThemePtr;
}
