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
};