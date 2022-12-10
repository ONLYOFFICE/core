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
                // этого не может быть...
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
