#pragma once
#include "TextFullSettings.h"
#include "SlidePersistAtom.h"

class CRecordSlideListWithText : public CRecordsContainer
{
public:
	enum Instances
    {
        CollectionOfSlides			= 0,
        CollectionOfMasterSlides	= 1,
        CollectionOfNotesSlides		= 2
    };

public:
	std::vector<CRecordSlidePersistAtom*> m_arSlides;
	std::vector<std::vector<CTextFullSettings>> m_arTextPlaceHolders;

	Instances m_Type;

public:
	
	CRecordSlideListWithText() : m_arSlides()
	{
		m_Type = CollectionOfSlides;
	}

	~CRecordSlideListWithText()
	{
	}

	virtual void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		m_Type = (Instances)m_oHeader.RecInstance;

		if (m_oHeader.IsContainer())
		{
			// а по-другому и быть не могло...
			DWORD lSymbolCount = 0;

			UINT lCurLen = 0;
			SRecordHeader oRec;

			while (lCurLen < m_oHeader.RecLen)
			{
				if (oRec.ReadFromStream(pStream) == FALSE)	
				{
					break;
				}
				
				IRecord* pRecord = CreateByType(oRec);

				if (RECORD_TYPE_STYLE_TEXTPROP_ATOM == oRec.RecType)
				{
					((CRecordStyleTextPropAtom*)(pRecord))->m_lCount = lSymbolCount;
				}
				else if (RECORD_TYPE_TEXTSPECINFO_ATOM == oRec.RecType)
				{
					((CRecordTextSpecInfoAtom*)(pRecord))->m_lCount = lSymbolCount;
				}

				pRecord->ReadFromStream(oRec, pStream);
				lCurLen += (8 + oRec.RecLen);

				if (RECORD_TYPE_SLIDE_PERSIST_ATOM == oRec.RecType)
				{
					m_arSlides.push_back((CRecordSlidePersistAtom*)pRecord);

					std::vector<CTextFullSettings> elm;
					m_arTextPlaceHolders.push_back(elm);
				}
				else
				{
					long nCurrentSlide = (long)m_arSlides.size() - 1;
					if (0 > nCurrentSlide)
						continue;
					
					CRecordTextHeaderAtom* pHeader = dynamic_cast<CRecordTextHeaderAtom*>(pRecord);
					if (NULL != pHeader)
					{
						CTextFullSettings oAttr;
						m_arTextPlaceHolders[nCurrentSlide].push_back(oAttr);
						m_arTextPlaceHolders[nCurrentSlide][m_arTextPlaceHolders[nCurrentSlide].size() - 1].m_nTextType = pHeader->m_nTextType;
						
						m_arRecords.push_back(pRecord);
						continue;
					}
					CRecordTextCharsAtom* pChars = dynamic_cast<CRecordTextCharsAtom*>(pRecord);
					CRecordTextBytesAtom* pBytes = dynamic_cast<CRecordTextBytesAtom*>(pRecord);

					long nCurrentTextHeader = (long)m_arTextPlaceHolders[nCurrentSlide].size() - 1;
					if (0 > nCurrentTextHeader)
						continue;

					// здесь еще настойки кроме текста
					if (NULL != pChars)
					{
						m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_strText = pChars->m_strText;
						lSymbolCount = (DWORD)pChars->m_strText.length();
					}
					else if (NULL != pBytes)
					{
						m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_strText = pBytes->m_strText;
						lSymbolCount = (DWORD)pBytes->m_strText.length();
					}					
					
					if (RECORD_TYPE_STYLE_TEXTPROP_ATOM == oRec.RecType)
					{
						m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_pTextStyleProp = 
								dynamic_cast<CRecordStyleTextPropAtom*>(pRecord);
					}
					if (RECORD_TYPE_TEXTSPECINFO_ATOM == oRec.RecType)
					{
						m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_pTextSpecInfo = 
								dynamic_cast<CRecordTextSpecInfoAtom*>(pRecord);
					}
					if (RECORD_TYPE_TEXTRULER_ATOM == oRec.RecType)
					{
						m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_pTextRuler = 
								dynamic_cast<CRecordTextRulerAtom*>(pRecord);
					}
					if (RECORD_TYPE_TXINTERACTIVEINFO_ATOM == oRec.RecType)
					{
						CRecordTextInteractiveInfoAtom* pTxRanges = dynamic_cast<CRecordTextInteractiveInfoAtom*>(pRecord);
						NSPresentationEditor::CTextRange oRange;
						oRange.m_lStart	= pTxRanges->m_lStart;
						oRange.m_lEnd	= pTxRanges->m_lEnd;
						m_arTextPlaceHolders[nCurrentSlide][nCurrentTextHeader].m_arRanges.push_back(oRange);
					}
				}

				m_arRecords.push_back(pRecord);
			}
		}
	}
	
	virtual CString ToString()
	{
		return CRecordsContainer::ToString();
	}
};