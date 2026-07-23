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
#include "ColorSchemeAtom.h"

using namespace PPT;
using namespace ODRAW;

void CRecordColorSchemeAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    NSStreamReader::Read(pStream, m_oBackgroundColor);
    NSStreamReader::Read(pStream, m_oTextAndLinesColor);
    NSStreamReader::Read(pStream, m_oShadowsColor);
    NSStreamReader::Read(pStream, m_oTitleTextColor);
    NSStreamReader::Read(pStream, m_oFillsColor);
    NSStreamReader::Read(pStream, m_oAccentColor);
    NSStreamReader::Read(pStream, m_oAccentAndHyperlinkColor);
    NSStreamReader::Read(pStream, m_oAccentAndFollowingHyperlinkColor);
}

void CRecordColorSchemeAtom::ToArray(std::vector<SColorAtom> *pArray)
{
    if (NULL != pArray)
    {
        pArray->push_back(m_oBackgroundColor);
        pArray->push_back(m_oTextAndLinesColor);
        pArray->push_back(m_oShadowsColor);
        pArray->push_back(m_oTitleTextColor);
        pArray->push_back(m_oFillsColor);
        pArray->push_back(m_oAccentColor);
        pArray->push_back(m_oAccentAndHyperlinkColor);
        pArray->push_back(m_oAccentAndFollowingHyperlinkColor);
    }
}

void CRecordColorSchemeAtom::ToArray(std::vector<CColor> *pArray)
{
    CColor oColor;
    if (NULL != pArray)
    {
        m_oBackgroundColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oTextAndLinesColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oShadowsColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oTitleTextColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oFillsColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oAccentColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oAccentAndHyperlinkColor.ToColor(&oColor);
        pArray->push_back(oColor);

        m_oAccentAndFollowingHyperlinkColor.ToColor(&oColor);
        pArray->push_back(oColor);
    }
}
