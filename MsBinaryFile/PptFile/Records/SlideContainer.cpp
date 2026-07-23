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
#include "SlideContainer.h"

using namespace PPT;

CRecordSlide::CRecordSlide()
{
    m_bExistsTransition			=	false;
    m_pSlideProgTagsContainer	=	NULL;
    m_lCountReferences			=	0;

    m_Index						=	-1;
    m_IndexUser					=	0;

    AddRef();
}

CRecordSlide::~CRecordSlide()
{
    Clear();
    RELEASEOBJECT ( m_pSlideProgTagsContainer );
}

void CRecordSlide::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;
    m_arRecords.clear();

    LONG lPosition = 0;
    StreamUtils::StreamPosition(lPosition, pStream);

    m_oHeader = oHeader;

    UINT lCurLen = 0;
    SRecordHeader oRec;

    while (lCurLen < m_oHeader.RecLen)
    {
        if (oRec.ReadFromStream(pStream) == FALSE)
        {
            break;
        }

        if ( RT_SlideShowSlideInfoAtom == oRec.RecType )
        {
            m_bExistsTransition	=	true;
            m_oSlideShowSlideInfoAtom.ReadFromStream ( oRec, pStream );

            lCurLen += (8 + oRec.RecLen);
            continue;
        }

        if ( RT_ProgTags == oRec.RecType )
        {
            m_pSlideProgTagsContainer =
                    new CRecordSlideProgTagsContainer();
            m_pSlideProgTagsContainer->ReadFromStream(oRec, pStream);

            lCurLen += (8 + oRec.RecLen);
            continue;
        }

        if (RT_VbaInfo == oRec.RecType)
        {
            auto pVbaInfo = new CRecordsContainer;
            pVbaInfo->ReadFromStream(oRec, pStream);
            m_arRecords.push_back(pVbaInfo);

            lCurLen += (8 + oRec.RecLen);
            continue;
        }

        IRecord* pRecord = CreateByType ( oRec, m_pCommonInfo );
        pRecord->ReadFromStream(oRec, pStream);
        m_arRecords.push_back(pRecord);

        lCurLen += (8 + oRec.RecLen);
    }
    if (lCurLen != m_oHeader.RecLen)
    {
        // need to figure out what this is!!!
        LONG lPosition = 0;
        StreamUtils::StreamSeek(lPosition + m_oHeader.RecLen, pStream);
    }
}

bool CRecordSlide::IsSlide()
{
    for (size_t nIndex = 0; nIndex < m_arRecords.size(); ++nIndex)
    {
        if ((RT_Slide			== m_arRecords[nIndex]->m_oHeader.RecType) ||
                (RT_MainMaster		== m_arRecords[nIndex]->m_oHeader.RecType) ||
                (RT_Notes			== m_arRecords[nIndex]->m_oHeader.RecType))
        {
            return true;
        }
    }
    return false;
}

void CRecordSlide::AddRef()
{
    ++m_lCountReferences;
}

void CRecordSlide::Release()
{
    --m_lCountReferences;
    if (0 == m_lCountReferences)
        delete this;
}

CRecordSlide *CRecordSlide::QueryInterface()
{
    AddRef();
    return this;
}
