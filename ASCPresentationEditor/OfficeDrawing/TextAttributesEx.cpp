#include "TextAttributesEx.h"
#include "Theme.h"

namespace NSPresentationEditor
{
	CString CTextAttributesEx::ToString(CGeomShapeInfo& oInfo, CMetricInfo& pMetricInfo, double dStartTime, double dEndTime, CTheme* pTheme, CLayout* pLayout)
	{
		return L"";
	}

	void CTextAttributesEx::RecalcParagraphsPPT(CTheme* pTheme)
	{
		for (int i = 0; i < m_arParagraphs.size(); ++i)
		{
			bool bIsBreak	= true;
			int lCountCFs	= m_arParagraphs[i].m_arSpans.size();
			for (int j = 0; j < lCountCFs; ++j)
			{
				int s_size		= m_arParagraphs[i].m_arSpans[j].m_strText.length();
				
				int lFound1		= m_arParagraphs[i].m_arSpans[j].m_strText.find((TCHAR)13);
				int lFound2		= m_arParagraphs[i].m_arSpans[j].m_strText.find((TCHAR)11);

				int lFoundEnter = (lFound1>=0 && lFound2>=0) ? (std::min)(lFound1, lFound2) : (lFound1>=0 ? lFound1 : (lFound2>=0 ? lFound2 : -1));

				if( lFoundEnter >= 0 && (s_size > 1 || (s_size == 1 && m_arParagraphs[i].m_arSpans.size() > 1)))
				{//"поделенный" параграф имеет единичный span - ваще то все равно остается возможность ошибки
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

					break;
				}
				else
				{
					bIsBreak = false;
				}
			}
		}

		if (NULL != pTheme)
		{
			// теперь снаследуем все для буллета
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

				if (-1 != m_lTextType && m_lTextType < 4 && pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel].is_init())
				{
					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
						hasBullet = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.hasBullet;

					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletColor.is_init())
						bulletColor = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletColor;

					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletChar.is_init() )
					{
						bulletFontRef = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletFontRef;
						bulletChar = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletChar;
					}
					
					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletSize.is_init())
						bulletSize = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletSize;

					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.leftMargin;

					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.indent;
		
					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.spaceBefore.is_init())
						spaceBefore = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.spaceBefore;
				}
				if (m_oLayoutStyles.m_pLevels[lLevel].is_init())
				{
					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
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

					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = m_oStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin;

					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = m_oStyles.m_pLevels[lLevel]->m_oPFRun.indent;
					
					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.spaceBefore.is_init())
						spaceBefore = m_oStyles.m_pLevels[lLevel]->m_oPFRun.spaceBefore;				
				}
				
				if (!m_arParagraphs[nIndexP].m_oPFRun.hasBullet.is_init())
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
				else
				{
					int g=0;
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

	void CTextAttributesEx::RecalcParagraphs(CTheme* pTheme)
	{
#ifdef PPT_DEF
		RecalcParagraphsPPT(pTheme);
#else
		size_t nCount = m_arParagraphs.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			m_arParagraphs[i].CheckErrors();
		}
#endif
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

				if (pPar->defaultTabSize.is_init() == false)
					pPar->defaultTabSize = m_oRuler.tabsStops[0];
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

