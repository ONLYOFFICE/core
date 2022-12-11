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
#include "DocInfoListContainer.h"

using namespace PPT;

void DocInfoListSubContainerOrAtom::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    switch (oHeader.RecType)
    {
    case RT_ProgTags:
    {
        m_record.reset(new CRecordDocProgTagsContainer);
        m_record->ReadFromStream(oHeader, pStream);
        break;
    }
    case RT_VbaInfo:
    {
        m_record.reset(new CRecordVBAInfoContainer);
        m_record->ReadFromStream(oHeader, pStream);
        break;
    }
//        case RT_OutlineViewInfo:
//        {

//        }
    default:
        StreamUtils::StreamSkip(oHeader.RecLen, pStream);
        break;
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
