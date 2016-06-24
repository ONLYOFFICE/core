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

class CRecordTextSpecInfoAtom : public CUnknownRecord
{
public:
	// нужно еще хранить позицию в стриме, 
	// указатель на начало
	LONG											m_lOffsetInStream;
	DWORD											m_lCount; 

	std::vector<NSPresentationEditor::CTextSIRun>	m_arrSIs;

	
	CRecordTextSpecInfoAtom()
	{
		m_lCount = 0;
		m_lOffsetInStream = 0;
	}

	~CRecordTextSpecInfoAtom()
	{
		m_lCount = 0;
	}

    void ReadFromStream(SRecordHeader & oHeader, POLE::Stream* pStream)
	{
		m_oHeader = oHeader;
		StreamUtils::StreamPosition(m_lOffsetInStream, pStream);

		DWORD lMemCount = 0;
		while (true)
		{
			NSPresentationEditor::CTextSIRun elm;
			m_arrSIs.push_back(elm);
			
			NSStreamReader::Read(pStream, m_arrSIs.back());
			lMemCount += m_arrSIs.back().lCount;

			int sz = pStream->tell() - m_lOffsetInStream;

			if (sz >= m_oHeader.RecLen)
				break;
		}

		// на всякий случай...
		// здесь когда текст сначала другой (т.е. например - placeholder в мастере) - 
		// то у нас неправильно выставился m_lCount... на число m_lCount пилюем .. берем структур si скока прописано
		StreamUtils::StreamSeek(m_lOffsetInStream + m_oHeader.RecLen, pStream);
	}

    void ApplyProperties(CTextAttributesEx* pText)
	{
		if (m_arrSIs.size() < 1) return;

		int pos_text = 0, pos_si = 0;
		int ind = 0;
		for (int i = 0; i < pText->m_arParagraphs.size(); i++)
		{
			if (ind >= m_arrSIs.size()) break;
			
			for (int j = 0 ; j < pText->m_arParagraphs[i].m_arSpans.size(); j++)
			{
				if (pos_text + pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() > pos_si + m_arrSIs[ind].lCount )
				{
					pos_si += m_arrSIs[ind].lCount;
					ind++;
				}
				if (ind >= m_arrSIs.size()) break;
				if (m_arrSIs[ind].bLang)
					pText->m_arParagraphs[i].m_arSpans[j].m_oRun.Language = m_arrSIs[ind].Lang;
				pos_text += pText->m_arParagraphs[i].m_arSpans[j].m_strText.length() ;
			}

		}
	}

};
