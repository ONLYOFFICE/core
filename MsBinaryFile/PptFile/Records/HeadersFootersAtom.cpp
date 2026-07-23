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
#include "HeadersFootersAtom.h"

using namespace PPT;

void CRecordHeadersFootersAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_nFormatID = StreamUtils::ReadWORD(pStream);

    BYTE nFlag			= StreamUtils::ReadBYTE(pStream);
    m_bHasDate			= ((nFlag & 0x01) == 0x01);
    m_bHasTodayDate		= ((nFlag & 0x02) == 0x02);
    m_bHasUserDate		= ((nFlag & 0x04) == 0x04);
    m_bHasSlideNumber	= ((nFlag & 0x08) == 0x08);
    m_bHasHeader		= ((nFlag & 0x10) == 0x10);
    m_bHasFooter		= ((nFlag & 0x20) == 0x20);

    StreamUtils::ReadBYTE(pStream);//reserved
}

void CRecordRoundTripHeaderFooterDefaults12Atom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    BYTE nFlag				= StreamUtils::ReadBYTE(pStream);

    m_bIncludeDate			= ((nFlag & 0x04) == 0x04);
    m_bIncludeFooter		= ((nFlag & 0x08) == 0x08);
    m_bIncludeHeader		= ((nFlag & 0x10) == 0x10);
    m_bIncludeSlideNumber	= ((nFlag & 0x20) == 0x20);
}

void CRecordMetaCharacterAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    if (oHeader.RecLen >=4)
    {
        m_nPosition =  StreamUtils::ReadDWORD(pStream);
    }
}

void CRecordRTFDateTimeMetaAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordGenericDateMetaAtom::ReadFromStream(oHeader, pStream);

    m_strFormat = StreamUtils::ReadStringA(pStream, 128);
}

void CRecordDateTimeMetaAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    POLE::uint64 lPosition = pStream->tell();

    CRecordGenericDateMetaAtom::ReadFromStream(oHeader, pStream);

    if (oHeader.RecLen == 8)
    {
        m_FormatID =  StreamUtils::ReadDWORD(pStream);
    }
    else if (oHeader.RecLen >4)
    {
        m_FormatID = StreamUtils::ReadBYTE(pStream);
        StreamUtils::StreamSeek((long)(lPosition + m_oHeader.RecLen), pStream);
    }
}

void CRecordHeadersFootersContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    CRecordsContainer::ReadFromStream(oHeader, pStream);

    for (size_t i = 0 ; i < m_arRecords.size(); i++)
    {
        switch(m_arRecords[i]->m_oHeader.RecType)
        {
        case 0x0FDA:
            m_oHeadersFootersAtom = dynamic_cast<CRecordHeadersFootersAtom *>(m_arRecords[i]);
            break;
        case 0xFBA:
            {
                CRecordCString *str = dynamic_cast<CRecordCString *>(m_arRecords[i]);
                switch(m_arRecords[i]->m_oHeader.RecInstance)
                {
                case 0x000: m_HeadersFootersString[0].push_back(str->m_strText);	break;
                case 0x001: m_HeadersFootersString[1].push_back(str->m_strText);	break;
                case 0x002: m_HeadersFootersString[2].push_back(str->m_strText);	break;
                }
            }break;
        }
    }
}
