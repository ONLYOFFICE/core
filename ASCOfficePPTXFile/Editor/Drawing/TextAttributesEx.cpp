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
#include "TextAttributesEx.h"
#include "Theme.h"

namespace NSPresentationEditor
{

	void CTextAttributesEx::RecalcParagraphsPPT()
	{
		for (int i = 0; i < m_arParagraphs.size(); ++i)
		{
			bool split_paragraph = false;
			for (int j = 0; j < m_arParagraphs[i].m_arSpans.size(); ++j)
			{
				int lCountCFs	= m_arParagraphs[i].m_arSpans.size();
				int s_size		= m_arParagraphs[i].m_arSpans[j].m_strText.length();
				
				int lFoundEnter = m_arParagraphs[i].m_arSpans[j].m_strText.find((TCHAR)13);

				if( !split_paragraph && lFoundEnter >= 0 && (s_size > 1 || (s_size == 1 && m_arParagraphs[i].m_arSpans.size() > 1)))
				{
					split_paragraph = true;
					// разбиваем параграф
					CParagraph oNewPar = m_arParagraphs[i];

					if (lCountCFs - (j + 1) > 0)
					{
						m_arParagraphs[i].m_arSpans.erase(m_arParagraphs[i].m_arSpans.begin() + (j + 1), m_arParagraphs[i].m_arSpans.begin() + lCountCFs );
					}
					
					LONG lCountTx = m_arParagraphs[i].m_arSpans[j].m_strText.length();
					m_arParagraphs[i].m_arSpans[j].m_strText.erase(lFoundEnter, lCountTx - lFoundEnter);

					if (j > 0)
						oNewPar.m_arSpans.erase(oNewPar.m_arSpans.begin(), oNewPar.m_arSpans.begin() + j);

					if (lFoundEnter == (lCountTx - 1))
						oNewPar.m_arSpans.erase(oNewPar.m_arSpans.begin(), oNewPar.m_arSpans.begin() + 1);
					else
						oNewPar.m_arSpans[0].m_strText.erase(0, lFoundEnter + 1);

					if (0 != oNewPar.m_arSpans.size())
						m_arParagraphs.insert(m_arParagraphs.begin() +i + 1, oNewPar);	
				}

				int lFoundBreak = m_arParagraphs[i].m_arSpans[j].m_strText.find((TCHAR)11);
				if( lFoundBreak >= 0)
				{
					// разбиваем span
					CSpan next	= m_arParagraphs[i].m_arSpans[j];
					
					next.m_strText								= next.m_strText.substr(lFoundBreak + 1);
					m_arParagraphs[i].m_arSpans[j].m_strText	= m_arParagraphs[i].m_arSpans[j].m_strText.substr(0, lFoundBreak - 1);
		
					if( lFoundBreak == 0)
					{
						m_arParagraphs[i].m_arSpans[j].m_strText.clear();
						m_arParagraphs[i].m_arSpans[j].m_bBreak	= true;

						m_arParagraphs[i].m_arSpans.insert( m_arParagraphs[i].m_arSpans.begin() + j + 1 , next);
					}
					else
					{
						CSpan br		= m_arParagraphs[i].m_arSpans[j];
						br.m_strText.clear(); 
						br.m_bBreak		= true;
						
						m_arParagraphs[i].m_arSpans.insert(	m_arParagraphs[i].m_arSpans.begin() + j	+ 1	, br);
						m_arParagraphs[i].m_arSpans.insert( m_arParagraphs[i].m_arSpans.begin() + j + 2 , next);
						j++;
					}
				}
			}
		}
	}

	void CTextAttributesEx::RecalcParagraphs(CTheme* pTheme)
	{
#ifdef PPT_DEF
		RecalcParagraphsPPT();
		ApplyThemeStyle(pTheme);
#else
		size_t nCount = m_arParagraphs.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			m_arParagraphs[i].CheckErrors();
		}
#endif
	}
	void CTextAttributesEx::ApplyThemeStyle(CTheme* pTheme)
	{
		if (NULL != pTheme)
		{
			size_t nCountPFs = m_arParagraphs.size();
			for (size_t nIndexP = 0; nIndexP < nCountPFs; ++nIndexP)
			{
				LONG lLevel = m_arParagraphs[nIndexP].m_lTextLevel;
                
				nullable_base<bool>		hasBullet;

				nullable_base<CColor>	bulletColor;
				nullable_base<WORD>		bulletFontRef;
				nullable_base<WORD>		bulletSize;
				nullable_base<WCHAR>	bulletChar;

				nullable_base<LONG>		indent;
				nullable_base<LONG>		margin;
				
				nullable_base<LONG>		spaceBefore;

				if (-1 != m_lStyleThemeIndex && m_lStyleThemeIndex < 4 && pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel].is_init())
				{
					if (pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
					{
						hasBullet = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.hasBullet;

						if (pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.bulletColor.is_init())
							bulletColor = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.bulletColor;

						if (pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.bulletChar.is_init() )
						{
							bulletFontRef = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.bulletFontRef;
							bulletChar = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.bulletChar;
						}
						
						if (pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.bulletSize.is_init())
							bulletSize = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.bulletSize;
					}

					if (pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.leftMargin;

					if (pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.indent;
		
					if (pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.spaceBefore.is_init())
						spaceBefore = pTheme->m_pStyles[m_lStyleThemeIndex].m_pLevels[lLevel]->m_oPFRun.spaceBefore;
				}
				if (m_oLayoutStyles.m_pLevels[lLevel].is_init())
				{
					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
					{
						hasBullet = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet;

						if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor.is_init())
							bulletColor = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor;

						if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletSize.is_init())
							bulletSize = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletSize;

						if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletChar.is_init() )
						{
							bulletChar = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletChar;
							bulletFontRef = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletFontRef;
						}
					}

					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin;

					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.indent;
				
					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.spaceBefore.is_init())
						spaceBefore = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.spaceBefore;	
				}
				if (m_oStyles.m_pLevels[lLevel].is_init())
				{
					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
					{
						hasBullet = m_oStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet;

						if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor.is_init())
							bulletColor = m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor;

						if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletChar.is_init())
						{
							bulletFontRef = m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletFontRef;
							bulletChar = m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletChar;
						}

						if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletSize.is_init())
							bulletSize = m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletSize;
					}
					

					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = m_oStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin;

					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = m_oStyles.m_pLevels[lLevel]->m_oPFRun.indent;
					
					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.spaceBefore.is_init())
						spaceBefore = m_oStyles.m_pLevels[lLevel]->m_oPFRun.spaceBefore;				
				}
				
				if (!m_arParagraphs[nIndexP].m_oPFRun.hasBullet.is_init())
				{
					m_arParagraphs[nIndexP].m_oPFRun.hasBullet = hasBullet;

					if (!m_arParagraphs[nIndexP].m_oPFRun.bulletColor.is_init())
						m_arParagraphs[nIndexP].m_oPFRun.bulletColor = bulletColor;

					if (!m_arParagraphs[nIndexP].m_oPFRun.bulletSize.is_init())
						m_arParagraphs[nIndexP].m_oPFRun.bulletSize = bulletSize;

					if (!m_arParagraphs[nIndexP].m_oPFRun.bulletChar.is_init())
					{
						m_arParagraphs[nIndexP].m_oPFRun.bulletChar = bulletChar;
						m_arParagraphs[nIndexP].m_oPFRun.bulletFontRef = bulletFontRef;
					}
				}

				if (!m_arParagraphs[nIndexP].m_oPFRun.leftMargin.is_init())
					m_arParagraphs[nIndexP].m_oPFRun.leftMargin = margin;

				if (!m_arParagraphs[nIndexP].m_oPFRun.indent.is_init())
					m_arParagraphs[nIndexP].m_oPFRun.indent = indent;

				if (!m_arParagraphs[nIndexP].m_oPFRun.spaceBefore.is_init())
					m_arParagraphs[nIndexP].m_oPFRun.spaceBefore = spaceBefore;

				m_arParagraphs[nIndexP].m_oPFRun.bulletFontProperties.reset();				
				if (m_arParagraphs[nIndexP].m_oPFRun.bulletFontRef.is_init())
				{
					m_arParagraphs[nIndexP].m_oPFRun.bulletFontProperties = new CFontProperties();

					int ref = m_arParagraphs[nIndexP].m_oPFRun.bulletFontRef.get();

					if (ref < pTheme->m_arFonts.size())
					{
						m_arParagraphs[nIndexP].m_oPFRun.bulletFontProperties->SetFont(pTheme->m_arFonts[ref]);
					}
				}

			}
		}

		size_t nCount = m_arParagraphs.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			m_arParagraphs[i].CheckErrors();
		}

		ApplyRuler(pTheme);		
	}
	void CTextAttributesEx::ApplyRuler(CTheme* pTheme)
	{
		if (m_oRuler.tabsStops.size() > 0 || m_oRuler.DefaultTabSize.is_init())//presentacio1.ppt слайд 2
		{
			for (long i = 0; i < 9 ;i++)
			{
				if (m_oStyles.m_pLevels[i].is_init() == false)
				{
					m_oStyles.m_pLevels[i] = pTheme->m_pStyles[0].m_pLevels[i];
				}
				if (m_oRuler.tabsStops.size() > 0)
				{
					m_oStyles.m_pLevels[i]->m_oPFRun.tabStops = m_oRuler.tabsStops;
				}

				if (m_oRuler.DefaultTabSize.is_init())
				{
					m_oStyles.m_pLevels[i]->m_oPFRun.defaultTabSize = m_oRuler.DefaultTabSize;
				}	
			}
		}

		size_t lCount = m_arParagraphs.size();
		for (size_t i = 0; i < lCount; ++i)
		{
			CTextPFRun* pPar = &m_arParagraphs[i].m_oPFRun;

			if (m_oRuler.tabsStops.size() > 0)
			{
				pPar->tabStops = m_oRuler.tabsStops;

				//if (pPar->defaultTabSize.is_init() == false)
				//	pPar->defaultTabSize = m_oRuler.tabsStops[0];
			}

			if (m_oRuler.DefaultTabSize.is_init())
			{
				pPar->defaultTabSize = m_oRuler.DefaultTabSize;
			}
			WORD lIndentLevel = (WORD)m_arParagraphs[i].m_lTextLevel;

			switch (lIndentLevel)
			{
			case 0:
			{
				if (m_oRuler.LeftMargin1.is_init()) 
					pPar->leftMargin = (LONG)m_oRuler.LeftMargin1.get();
				if (m_oRuler.Indent1.is_init()) 
					pPar->indent = (LONG)m_oRuler.Indent1.get();
				
				if (pPar->indent.is_init() && pPar->leftMargin.is_init())
					pPar->indent = pPar->indent.get() - pPar->leftMargin.get();
				break;
			}
			case 1:
			{
				if (m_oRuler.LeftMargin2.is_init()) 
					pPar->leftMargin = (LONG)m_oRuler.LeftMargin2.get();
				if (m_oRuler.Indent2.is_init()) 
					pPar->indent  = (LONG)m_oRuler.Indent2.get();
				
				if (pPar->indent.is_init() && pPar->leftMargin.is_init())
					pPar->indent = pPar->indent.get() - pPar->leftMargin.get();
				break;
			}
			case 2:
			{
				if (m_oRuler.LeftMargin3.is_init()) 
					pPar->leftMargin = (LONG)m_oRuler.LeftMargin3.get();
				if (m_oRuler.Indent3.is_init()) 
					pPar->indent  = (LONG)m_oRuler.Indent3.get();
				
				if (pPar->indent.is_init() && pPar->leftMargin.is_init())
					pPar->indent = pPar->indent.get() - pPar->leftMargin.get();
				break;
			}
			case 3:
			{
				if (m_oRuler.LeftMargin4.is_init()) 
					pPar->leftMargin = (LONG)m_oRuler.LeftMargin4.get();
				if (m_oRuler.Indent4.is_init()) 
					pPar->indent  = (LONG)m_oRuler.Indent4.get();
				
				if (pPar->indent.is_init() && pPar->leftMargin.is_init())
					pPar->indent = pPar->indent.get() - pPar->leftMargin.get();
				break;
			}
			case 4:
			{
				if (m_oRuler.LeftMargin5.is_init()) 
					pPar->leftMargin = (LONG)m_oRuler.LeftMargin5.get();
				if (m_oRuler.Indent5.is_init()) 
					pPar->indent  = (LONG)m_oRuler.Indent5.get();
				
				if (pPar->indent.is_init() && pPar->leftMargin.is_init())
					pPar->indent = pPar->indent.get() - pPar->leftMargin.get();
				break;
			}
			default:
				break;
			}
		}
	}

	void CTextAttributesEx::ApplyRuler	(CTextPFRun* pPar, WORD lIndentLevel)
	{
		switch (lIndentLevel)
		{
		case 0:
		{
			if (m_oRuler.LeftMargin1.is_init()) 
				pPar->leftMargin = (LONG)m_oRuler.LeftMargin1.get();
			if (m_oRuler.Indent1.is_init()) 
				pPar->indent = (LONG)m_oRuler.Indent1.get();
			
			if (pPar->indent.is_init() && pPar->leftMargin.get())
				pPar->indent.get() -= pPar->leftMargin.get();
			break;
		}
		case 1:
		{
			if (m_oRuler.LeftMargin2.is_init()) 
				pPar->leftMargin = (LONG)m_oRuler.LeftMargin2.get();
			if (m_oRuler.Indent2.is_init()) 
				pPar->indent  = (LONG)m_oRuler.Indent2.get();
			
			if (pPar->indent.is_init() && pPar->leftMargin.get())
				pPar->indent.get() -= pPar->leftMargin.get();
			break;
		}
		case 2:
		{
			if (m_oRuler.LeftMargin3.is_init()) 
				pPar->leftMargin = (LONG)m_oRuler.LeftMargin3.get();
			if (m_oRuler.Indent3.is_init()) 
				pPar->indent  = (LONG)m_oRuler.Indent3.get();
			
			if (pPar->indent.is_init() && pPar->leftMargin.get())
				pPar->indent.get() -= pPar->leftMargin.get();
			break;
		}
		case 3:
		{
			if (m_oRuler.LeftMargin4.is_init()) 
				pPar->leftMargin = (LONG)m_oRuler.LeftMargin4.get();
			if (m_oRuler.Indent4.is_init()) 
				pPar->indent  = (LONG)m_oRuler.Indent4.get();
			
			if (pPar->indent.is_init() && pPar->leftMargin.get())
				pPar->indent.get() -= pPar->leftMargin.get();
			break;
		}
		case 4:
		{
			if (m_oRuler.LeftMargin5.is_init()) 
				pPar->leftMargin = (LONG)m_oRuler.LeftMargin5.get();
			if (m_oRuler.Indent5.is_init()) 
				pPar->indent  = (LONG)m_oRuler.Indent5.get();
			
			if (pPar->indent.is_init() && pPar->leftMargin.get())
				pPar->indent.get() -= pPar->leftMargin.get();
			break;
		}
		default:
			break;
		};
	}

}

