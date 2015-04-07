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

	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};