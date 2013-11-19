#pragma once
#include "..\Reader\Records.h"

class CRecordDocument : public CRecordsContainer
{
public:
	CAtlArray<SSlidePersist> m_arMasterPersists;
	CAtlArray<SSlidePersist> m_arNotePersists;
	CAtlArray<SSlidePersist> m_arSlidePersists;

public:
	
	CRecordDocument() : m_arMasterPersists(), m_arNotePersists(), m_arSlidePersists()
	{
	}

	~CRecordDocument()
	{
		m_arMasterPersists.RemoveAll();
		m_arNotePersists.RemoveAll();
		m_arSlidePersists.RemoveAll();
	}

	/*virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		m_arMasterPersists.RemoveAll();
		m_arNotePersists.RemoveAll();
		m_arSlidePersists.RemoveAll();		
		
		m_arRecords.RemoveAll();

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
				CAtlArray<SSlidePersist>* pArray = NULL;

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
				
				size_t nCountItems = pAtom->m_arSlides.GetCount();
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

	virtual void ReadFromStream(SRecordHeader oHeader, IStream* pStream)
	{
		m_oHeader = oHeader;

		m_arMasterPersists.RemoveAll();
		m_arNotePersists.RemoveAll();
		m_arSlidePersists.RemoveAll();		
		
		m_arRecords.RemoveAll();

		CRecordsContainer::ReadFromStream(oHeader, pStream);

		CSimpleArray<CRecordSlideListWithText*> oArraySlideWithText;
		this->GetRecordsByType(&oArraySlideWithText, true, false);

		for (int nIndexList = 0; nIndexList < oArraySlideWithText.GetSize(); ++nIndexList)
		{
			CRecordSlideListWithText* pAtom = oArraySlideWithText[nIndexList];
			CAtlArray<SSlidePersist>* pArray = NULL;

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
			
			size_t nCountItems = pAtom->m_arSlides.GetCount();
			for (size_t index = 0; index < nCountItems; ++index)
			{
				SSlidePersist oPersist;
				oPersist.m_nPsrRef = pAtom->m_arSlides[index]->m_nPsrRef;
				oPersist.m_nSlideID = pAtom->m_arSlides[index]->m_nSlideID;

				oPersist.m_arTextAttrs.Append(pAtom->m_arTextPlaceHolders[index]);

				pArray->Add(oPersist);
			}
		}
	}

	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};