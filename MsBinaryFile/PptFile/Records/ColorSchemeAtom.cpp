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
