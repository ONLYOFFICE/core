/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
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
#pragma once
#include "../Reader/Records.h"

class CRecordDocument : public CRecordsContainer
{
public:
	std::vector<SSlidePersist> m_arMasterPersists;
	std::vector<SSlidePersist> m_arNotePersists;
	std::vector<SSlidePersist> m_arSlidePersists;

public:
	
	CRecordDocument() : m_arMasterPersists(), m_arNotePersists(), m_arSlidePersists()
	{
	}

	~CRecordDocument()
	{
		m_arMasterPersists.clear();
		m_arNotePersists.clear();
		m_arSlidePersists.clear();
	}

	/*virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_arMasterPersists.clear();
		m_arNotePersists.clear();
		m_arSlidePersists.clear();		
		
		m_arRecords.clear();

		UINT lCurLen = 0;
		ULONG lReadLen = 0;
		SRecordHeader oRec;
		while (lCurLen < m_oHeader.RecLen)
		{
			pStream->Read(&oRec, sizeof(oRec), &lReadLen);
			if (0 >= lReadLen)
			{
				break;
			}

			if (RECORD_TYPE_SLIDELISTWITHTEXT == oRec.RecType)
			{
				CRecordSlideListWithText* pAtom = new CRecordSlideListWithText();
				pAtom->ReadFromStream(oRec, pStream);
				std::vector<SSlidePersist>* pArray = NULL;

				switch (pAtom->m_Type)
                {
				case CRecordSlideListWithText::CollectionOfMasterSlides:
					{
						pArray = &m_arMasterPersists;
                        break;
					}

				case CRecordSlideListWithText::CollectionOfNotesSlides:
					{
						pArray = &m_arNotePersists;
                        break;
					}

				case CRecordSlideListWithText::CollectionOfSlides:
					{
						pArray = &m_arSlidePersists;
                        break;
					}
				default:
					{
						// этого не может быть...
						lCurLen += (8 + oRec.RecLen);
						continue;
					}
                };
				
				size_t nCountItems = pAtom->m_arSlides.size();
				for (size_t index = 0; index < nCountItems; ++index)
				{
					SSlidePersist oPersist;
					oPersist.m_nPsrRef = pAtom->m_arSlides[index]->m_nPsrRef;
					oPersist.m_nSlideID = pAtom->m_arSlides[index]->m_nSlideID;

					oPersist.m_arTextAttrs.Append(pAtom->m_arTextPlaceHolders[index]);

					pArray->Add(oPersist);
				}

				delete pAtom;
			}
			else
			{
				IRecord* pRecord = CreateByType(oRec);
				pRecord->ReadFromStream(oRec, pStream);
				m_arRecords.Add(pRecord);
			}
			lCurLen += (8 + oRec.RecLen);
		}
	}*/

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;

		m_arMasterPersists.clear();
		m_arNotePersists.clear();
		m_arSlidePersists.clear();		
		
		m_arRecords.clear();

		CRecordsContainer::ReadFromStream(oHeader, pStream);

		std::vector<CRecordSlideListWithText*> oArraySlideWithText;
		this->GetRecordsByType(&oArraySlideWithText, true, false);

		for (int nIndexList = 0; nIndexList < oArraySlideWithText.size(); ++nIndexList)
		{
			CRecordSlideListWithText* pAtom = oArraySlideWithText[nIndexList];
			std::vector<SSlidePersist>* pArray = NULL;

			switch (pAtom->m_Type)
            {
			case CRecordSlideListWithText::CollectionOfMasterSlides:
				{
					pArray = &m_arMasterPersists;
                    break;
				}

			case CRecordSlideListWithText::CollectionOfNotesSlides:
				{
					pArray = &m_arNotePersists;
                    break;
				}

			case CRecordSlideListWithText::CollectionOfSlides:
				{
					pArray = &m_arSlidePersists;
                    break;
				}
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

				oPersist.m_arTextAttrs.insert(oPersist.m_arTextAttrs.end(), pAtom->m_arTextPlaceHolders[index].begin(),pAtom->m_arTextPlaceHolders[index].end());

				pArray->push_back(oPersist);
			}
		}
	}
};