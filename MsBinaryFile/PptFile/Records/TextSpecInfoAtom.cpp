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
#include "TextSpecInfoAtom.h"

using namespace PPT;

CRecordTextSpecInfoAtom::CRecordTextSpecInfoAtom()
{
    m_lCount = 0;
    m_lOffsetInStream = 0;
}

CRecordTextSpecInfoAtom::~CRecordTextSpecInfoAtom()
{
    m_lCount = 0;
}

void CRecordTextSpecInfoAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

    _UINT32 lMemCount = 0;
    while (true)
    {
        PPT::CTextSIRun elm;
        m_arrSIs.push_back(elm);

        NSStreamReader::Read(pStream, m_arrSIs.back());
        lMemCount += m_arrSIs.back().lCount;

        long sz = (long)(pStream->tell() - m_lOffsetInStream);

        if (sz >= (long)m_oHeader.RecLen)
            break;
    }

    // just in case...
    // here when the text is initially different (e.g. placeholder in master) -
    // then m_lCount is set incorrectly... we ignore m_lCount and take as many si structures as specified
    StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
}

void CRecordTextSpecInfoAtom::ApplyProperties(CTextAttributesEx *pText)
{
    if (m_arrSIs.size() < 1) return;

    int pos_text = 0, pos_si = 0;
    size_t ind = 0;

    for (size_t i = 0; i < pText->m_arParagraphs.size(); i++)
    {
        if (ind >= m_arrSIs.size()) break;

        for (size_t j = 0 ; j < pText->m_arParagraphs[i].m_arSpans.size(); j++)
        {
            if (pos_text + pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() > pos_si + m_arrSIs[ind].lCount )
            {
                pos_si += m_arrSIs[ind].lCount;
                ind++;
            }
            if (ind >= m_arrSIs.size()) break;
            if (m_arrSIs[ind].bLang)
                pText->m_arParagraphs[i].m_arSpans[j].m_oRun.Language = m_arrSIs[ind].Lang;
            pos_text += pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() ;
        }

    }
}
