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
#include "OfficeArtClientData.h"

using namespace PPT;

void CRecordShapeProgBinaryTagContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    SRecordHeader ReadHeader;
    m_rec.ReadFromStream(ReadHeader, pStream);

    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

CRecordOfficeArtClientData::~CRecordOfficeArtClientData()
{
    CRecordsContainer::Clear();
    for ( size_t i = 0; i < m_rgShapeClientRoundtripData.size(); ++i )
        RELEASEOBJECT ( m_rgShapeClientRoundtripData[i] );
}

CRecordShapeProgBinaryTagSubContainerOrAtom *CRecordOfficeArtClientData::getProgTag(const std::wstring &tagname)
{
    for (auto* progtag : m_rgShapeClientRoundtripData)
    {
        if ((progtag->m_pTagName) && (progtag->m_pTagName->m_strText == tagname))
            return progtag;
    }

    return nullptr;
}

void CRecordOfficeArtClientData::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos			=	0;
    StreamUtils::StreamPosition ( lPos, pStream );

    UINT lCurLen		=	0;

    SRecordHeader ReadHeader;

    while ( lCurLen < m_oHeader.RecLen )
    {
        if ( ReadHeader.ReadFromStream(pStream) == false)
            break;

        lCurLen += 8 + ReadHeader.RecLen;

        switch (ReadHeader.RecType)
        {
            case RT_ProgTags:
            {
                // ShapeProgBinaryTagContainer
                ReadHeader.ReadFromStream(pStream);

                auto pRec = new CRecordShapeProgBinaryTagSubContainerOrAtom;
                pRec->m_pCommonInfo = m_pCommonInfo;
                pRec->ReadFromStream(ReadHeader, pStream);

                m_rgShapeClientRoundtripData.push_back(pRec);
            }break;
            case RT_VbaInfo:
            {
                auto pRec = new CRecordsContainer;
                pRec->m_pCommonInfo = m_pCommonInfo;
                pRec->ReadFromStream(ReadHeader, pStream);                
            }break;
            default:
            {
                IRecord* pRecord = CreateByType(ReadHeader, m_pCommonInfo);
                pRecord->ReadFromStream(ReadHeader, pStream);

                m_arRecords.push_back(pRecord);

            }break;
        }

    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}
