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
#include "DocumentRecords.h"
#include "RecordsIncluder.h"

using namespace PPT;

CRecordDocument::CRecordDocument()
{

}

CRecordDocument::~CRecordDocument()
{
    m_arMasterPersists.clear();
    m_arNotePersists.clear();
    m_arSlidePersists.clear();
}

void CRecordDocument::ReadFromStream(SRecordHeader &oHeader, POLE::Stream *pStream)
{
    m_oHeader = oHeader;

    m_arMasterPersists.clear();
    m_arNotePersists.clear();
    m_arSlidePersists.clear();

    m_arRecords.clear();

    CRecordsContainer::ReadFromStream(oHeader, pStream);

    std::vector<CRecordSlideListWithText*> oArraySlideWithText;
    this->GetRecordsByType(&oArraySlideWithText, true, false);

    for (size_t nIndexList = 0; nIndexList < oArraySlideWithText.size(); ++nIndexList)
    {
        CRecordSlideListWithText* pAtom = oArraySlideWithText[nIndexList];
        std::vector<SSlidePersist>* pArray = NULL;

        switch (pAtom->m_Type)
        {
            case CRecordSlideListWithText::CollectionOfMasterSlides:
            {
                pArray = &m_arMasterPersists;
            }break;
            case CRecordSlideListWithText::CollectionOfNotesSlides:
            {
                pArray = &m_arNotePersists;
            }break;
            case CRecordSlideListWithText::CollectionOfSlides:
            {
                pArray = &m_arSlidePersists;
            }break;
            default:
            {
                // this cannot happen...
                continue;
            }
        };

        size_t nCountItems = pAtom->m_arSlides.size();
        for (size_t index = 0; index < nCountItems; ++index)
        {
            SSlidePersist oPersist;
            oPersist.m_nPsrRef = pAtom->m_arSlides[index]->m_nPsrRef;
            oPersist.m_nSlideID = pAtom->m_arSlides[index]->m_nSlideID;

//            oPersist.m_arTextAttrs.insert(oPersist.m_arTextAttrs.end(), pAtom->m_arTextPlaceHolders[index].begin(),pAtom->m_arTextPlaceHolders[index].end());
            for (auto& textPlaceHolder : pAtom->m_arTextPlaceHolders[index])
                            oPersist.m_arTextAttrs.push_back(&textPlaceHolder);

            pArray->push_back(oPersist);
        }
    }
}
