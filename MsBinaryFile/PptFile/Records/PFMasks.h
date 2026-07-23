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
#include "../Reader/Records.h"

namespace PPT
{
struct PFMasks
{
    bool m_hasBullet;
    bool m_bulletHasFont;
    bool m_bulletHasColor;
    bool m_bulletHasSize;
    bool m_bulletFont;
    bool m_bulletColor;
    bool m_bulletSize;
    bool m_bulletChar;
    bool m_leftMargin;
    // unused
    bool m_indent;
    bool m_align;
    bool m_lineSpacing;
    bool m_spaceBefore;
    bool m_spaceAfter;
    bool m_defaultTabSize;
    bool m_fontAlign;
    bool m_charWrap;
    bool m_wordWrap;
    bool m_overflow;
    bool m_tabStops;
    bool m_textDirection;
    // reserved1
    bool m_bulletBlip;
    bool m_bulletScheme;
    bool m_bulletHasScheme;


    void ReadFromStream(POLE::Stream* pStream);
};
}
