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
#include "DocInfoListContainer.h"

using namespace PPT;

void DocInfoListSubContainerOrAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    switch (oHeader.RecType)
    {
        case RT_ProgTags:
        {
            m_record.reset(new CRecordDocProgTagsContainer);
            m_record->m_pCommonInfo = m_pCommonInfo;
            m_record->ReadFromStream(oHeader, pStream);        
        }break;
        case RT_VbaInfo:
        {
            m_record.reset(new CRecordVBAInfoContainer);
            m_record->m_pCommonInfo = m_pCommonInfo;
            m_record->ReadFromStream(oHeader, pStream);
        }break;
    // RT_OutlineViewInfo:
        default:
        {
            StreamUtils::StreamSkip(oHeader.RecLen, pStream);
        }break;
    }
}

CRecordDocInfoListContainer::CRecordDocInfoListContainer()
{

}

CRecordDocInfoListContainer::~CRecordDocInfoListContainer()
{
    for (auto child : m_rgChildRec)
    {
        RELEASEOBJECT(child)
    }
}

void CRecordDocInfoListContainer::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    LONG lPos; StreamUtils::StreamPosition(lPos, pStream);
    LONG lCurLen = 0;
    SRecordHeader ReadHeader;

    while (lCurLen < (LONG)m_oHeader.RecLen)
    {

        if (!ReadHeader.ReadFromStream(pStream))
        {
            break;
        }

        lCurLen += 8 + ReadHeader.RecLen;

        auto pRec = new DocInfoListSubContainerOrAtom;
        pRec->m_pCommonInfo = m_pCommonInfo;

        pRec->ReadFromStream(ReadHeader, pStream);

        m_rgChildRec.push_back(pRec);
    }
    StreamUtils::StreamSeek(lPos + m_oHeader.RecLen, pStream);
}

IRecord *CRecordDocInfoListContainer::getDocBinaryTagExtension(const std::wstring &extVersion)
{
    for (auto* pChild : m_rgChildRec)
    {
        if(pChild == nullptr || pChild->m_record.IsInit() == false)
            continue;

        auto* pDocProgTagsCont = dynamic_cast<CRecordDocProgTagsContainer*>(pChild->m_record.GetPointer());
        if (pDocProgTagsCont == nullptr)
            continue;

        return pDocProgTagsCont->getDocBinaryTagExtension(extVersion);
    }
    return nullptr;
}

CRecordVBAInfoAtom *CRecordDocInfoListContainer::getVBAInfoAtom() const
{
    for (const auto* pChild : m_rgChildRec)
    {
        if (!pChild || !pChild->m_record.IsInit())
            continue;
        IRecord* pRecord = pChild->m_record.GetPointer();
        auto* pVBA = dynamic_cast<CRecordVBAInfoContainer*>(pRecord);
        if (pVBA)
            return static_cast<CRecordVBAInfoAtom*>(pVBA->m_arRecords[0]);
    }

    return nullptr;
}
