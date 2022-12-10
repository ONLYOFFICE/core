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
