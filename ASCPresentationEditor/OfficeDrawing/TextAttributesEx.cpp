//#include "stdafx.h"
#include "TextAttributesEx.h"
#include "Theme.h"

namespace NSPresentationEditor
{
	CString CTextAttributesEx::ToString(CGeomShapeInfo& oInfo, CMetricInfo& pMetricInfo, double dStartTime, double dEndTime, CTheme* pTheme, CLayout* pLayout)
	{
		m_oBounds.left		= (LONG)oInfo.m_dLeft;
		m_oBounds.top		= (LONG)oInfo.m_dTop;
		m_oBounds.right		= (LONG)(oInfo.m_dLeft + oInfo.m_dWidth);
		m_oBounds.bottom	= (LONG)(oInfo.m_dTop + oInfo.m_dHeight);

		double dRight		= oInfo.m_dLeft + oInfo.m_dWidth;
		double dLeft		= oInfo.m_dLeft;
		if (2 == m_lWrapMode)
		{
			LONG lAlign = 0;
			if (m_arParagraphs.size() > 0 && m_arParagraphs[0].m_oPFRun.textAlignment.is_init())
				lAlign = m_arParagraphs[0].m_oPFRun.textAlignment.get();
			else
				lAlign = m_oAttributes.m_nTextAlignHorizontal;
			
			switch (lAlign)
			{
			case 1:
				{
					// center
					dLeft	-= pMetricInfo.m_lMillimetresHor;
					dRight	+= pMetricInfo.m_lMillimetresHor;
					break;
				}
			default:
				{
					dRight	= pMetricInfo.m_lMillimetresHor;
					break;
				}
			};
		}

		CString strText = _T("");
		// add Text Param
		strText.Format(_T("<ImagePaint-DrawTextEx left='%lf' top='%lf' right='%lf' bottom='%lf' angle='%lf' flags='%d' "), 
			dLeft, oInfo.m_dTop, dRight, oInfo.m_dTop + oInfo.m_dHeight, oInfo.m_dRotate, oInfo.GetFlags());

		RecalcParagraphs();

		CString strInfoPars = _T("");
		CString strInfoCFs	= _T("");
		CString sText		= _T("");

		size_t nCountPars = m_arParagraphs.size();
		for (size_t nIndexPF = 0; nIndexPF < nCountPars; ++nIndexPF)
		{
			LONG lLevel = m_arParagraphs[nIndexPF].m_lTextLevel;
			
			CTextPFRun oPFRun;
			if (-1 != m_lTextType && pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel].is_init())
			{
				oPFRun.ApplyAfter(pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun);
			}
			if (m_oLayoutStyles.m_pLevels[lLevel].is_init())
			{
				oPFRun.ApplyAfter(m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun);
			}
			if (m_oStyles.m_pLevels[lLevel].is_init())
			{
				oPFRun.ApplyAfter(m_oStyles.m_pLevels[lLevel]->m_oPFRun);
			}
			oPFRun.ApplyAfter(m_arParagraphs[nIndexPF].m_oPFRun);

			if (0 == nIndexPF)
			{
				if (oPFRun.spaceBefore.is_init())
					oPFRun.spaceBefore = (LONG)0;
			}

			#ifdef PPT_DEF
			#ifndef PPTX_DEF
				CorrectRuler(&oPFRun, (WORD)lLevel);
			#endif
			#endif


			oPFRun.bIsOneLine = m_arParagraphs[nIndexPF].m_oPFRun.bIsOneLine;

			LONG lCountParMem = 0;
			
			size_t nCountCFs = m_arParagraphs[nIndexPF].m_arSpans.size();

			if (0 == nCountCFs)
			{
				CTextCFRun oCFRun;
				if (-1 != m_lTextType && pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel].is_init())
				{
					oCFRun.ApplyAfter(pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oCFRun);
				}
				if (-1 != m_lFontRef)
				{
					oCFRun.Typeface = (WORD)m_lFontRef;
					if (!oCFRun.FontProperties.is_init())
						oCFRun.FontProperties = new CFontProperties();
					oCFRun.FontProperties->SetFont(pTheme->m_arFonts[m_lFontRef]);
				}
				if (m_oLayoutStyles.m_pLevels[lLevel].is_init())
				{
					oCFRun.ApplyAfter(m_oLayoutStyles.m_pLevels[lLevel]->m_oCFRun);
				}
				if (m_oStyles.m_pLevels[lLevel].is_init())
				{
					oCFRun.ApplyAfter(m_oStyles.m_pLevels[lLevel]->m_oCFRun);
				}
				sText += _T("\n");
				strInfoCFs += oCFRun.ToString(1);
				lCountParMem += 1;
			}

			for (size_t nIndexCF = 0; nIndexCF < nCountCFs; ++nIndexCF)
			{
				LONG lCountCFMem = m_arParagraphs[nIndexPF].m_arSpans[nIndexCF].m_strText.GetLength();
				sText += m_arParagraphs[nIndexPF].m_arSpans[nIndexCF].m_strText;

				CTextCFRun oCFRun;
				if (-1 != m_lTextType && pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel].is_init())
				{
					oCFRun.ApplyAfter(pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oCFRun);
				}
				if (-1 != m_lFontRef)
				{
					oCFRun.Typeface = (WORD)m_lFontRef;
					if (!oCFRun.FontProperties.is_init())
						oCFRun.FontProperties = new CFontProperties();
					oCFRun.FontProperties->SetFont(pTheme->m_arFonts[m_lFontRef]);
				}
				if (m_oLayoutStyles.m_pLevels[lLevel].is_init())
				{
					oCFRun.ApplyAfter(m_oLayoutStyles.m_pLevels[lLevel]->m_oCFRun);
				}
				if (m_oStyles.m_pLevels[lLevel].is_init())
				{
					oCFRun.ApplyAfter(m_oStyles.m_pLevels[lLevel]->m_oCFRun);
				}
				oCFRun.ApplyAfter(m_arParagraphs[nIndexPF].m_arSpans[nIndexCF].m_oRun);

				strInfoCFs += oCFRun.ToString(lCountCFMem);

				lCountParMem += lCountCFMem;
			}

			strInfoPars += oPFRun.ToString(lCountParMem);
		}

		// вот здесь нужно разрулить пререход на новую строку	
		sText.Replace((WCHAR)(11), '\n');
		//sText.Replace((WCHAR)(13), '\n');

		// теперь разберемся с хмл
		NormalizeString(sText);
		
		strText += (_T("text='") + sText + _T("' "));

		strText += (_T("vertical='") + NSPresentationEditor::BoolToString(m_bVertical == TRUE) + _T("' "));
		strText += (_T("righttoleft='") + NSPresentationEditor::BoolToString(m_bRightToLeft == TRUE) + _T("' "));

		strText += (_T("widthmm='") + NSPresentationEditor::ToString(pMetricInfo.m_lMillimetresHor) + _T("' "));
		strText += (_T("heightmm='") + NSPresentationEditor::ToString(pMetricInfo.m_lMillimetresVer) + _T("' "));
		strText += (_T("widthl='") + NSPresentationEditor::ToString(pMetricInfo.m_lUnitsHor) + _T("' "));
		strText += (_T("heightl='") + NSPresentationEditor::ToString(pMetricInfo.m_lUnitsVer) + _T("' "));

		strText += _T(">");

		strText += m_oAttributes.ToString();

		strText += strInfoPars;
		strText += strInfoCFs;
		
		CString strTimeLine = _T("");
		strTimeLine.Format(_T("<timeline type='1' begin='%lf' end='%lf' fadein='0' fadeout='0' completeness='1.0'/>"),
				dStartTime, dEndTime);

		return strText + strTimeLine + _T("</ImagePaint-DrawTextEx>");
	}

	CString CTextAttributesEx::ToHTML(CGeomShapeInfo& oInfo, CMetricInfo& pMetricInfo, double dStartTime, double dEndTime, const CElemInfo& oElemInfo, CTheme* pTheme, CLayout* pLayout)
	{
		m_oBounds.left		= (LONG)oInfo.m_dLeft;
		m_oBounds.top		= (LONG)oInfo.m_dTop;
		m_oBounds.right		= (LONG)(oInfo.m_dLeft + oInfo.m_dWidth);
		m_oBounds.bottom	= (LONG)(oInfo.m_dTop + oInfo.m_dHeight);

		double dRight		= oInfo.m_dLeft + oInfo.m_dWidth;
		double dLeft		= oInfo.m_dLeft;
		//if (2 == m_lWrapMode)
		//{
		//	LONG lAlign = 0;
		//	if (m_arPFs.size() > 0)
		//		lAlign = m_arPFs[0].textAlignment;
		//	else
		//		lAlign = m_oAttributes.m_nTextAlignHorizontal;
		//	
		//	switch (lAlign)
		//	{
		//	case 1:
		//		{
		//			// center
		//			dLeft	-= pMetricInfo.m_lMillimetresHor;
		//			dRight	+= pMetricInfo.m_lMillimetresHor;
		//			break;
		//		}
		//	default:
		//		{
		//			dRight	= pMetricInfo.m_lMillimetresHor;
		//			break;
		//		}
		//	};
		//}

		CString strHTML = _T("");

		LONG _lLeft		= (LONG)(dLeft * 96 / 25.4);
		LONG _lWidth	= (LONG)((dRight - dLeft) * 96 / 25.4);
		LONG _lTop		= (LONG)(oInfo.m_dTop * 96 / 25.4);
		LONG _lHeight	= (LONG)(oInfo.m_dHeight * 96 / 25.4);

		CString strText = _T("");
		// add Text Param
		strText.Format(_T("<text left='%d' top='%d' width='%d' height='%d' angle='%lf' background='%d' changeable='%d'>"), 
			_lLeft, _lTop, _lWidth, _lHeight, oInfo.m_dRotate, (int)(oElemInfo.m_bIsBackground ? 1 : 0), (int)(oElemInfo.m_bIsChangeable ? 1 : 0));

		strHTML += strText;
		strHTML += _T("<body>");

		RecalcParagraphs(pTheme);

		#ifdef PPT_DEF
		#ifndef PPTX_DEF
			// теперь пробежимся и проставим все маргины и инденты
			CorrectRuler();
		#endif
		#endif

		size_t nCountPars = m_arParagraphs.size();
		for (size_t i = 0; i < nCountPars; ++i)
		{
			CParagraph* pParagraph = &m_arParagraphs[i];
			LONG lLevel = pParagraph->m_lTextLevel;
			
			CString strClass = _T("");
			
			CTextPFRun oPFRun;

			bool bIsBullet = (pParagraph->m_oPFRun.hasBullet.is_init() && pParagraph->m_oPFRun.hasBullet.get()) ? true : false;

			if (-1 != m_lTextType)
			{
				CString strClassTheme = _T("");
				strClassTheme.Format(_T("s%d_lvl%d_pf"), m_lTextType, lLevel);
				strClass += strClassTheme;
			}
			if (-1 != m_lPlaceholderType)
			{
				CString strPlaceholderStyle = _T("");
				strPlaceholderStyle.Format(_T("t%d_lvl%d_pf"), m_lPlaceholderType, lLevel);

				strClass += _T(" ");
				strClass += strPlaceholderStyle;
			}
			if (m_oStyles.m_pLevels[lLevel].is_init())
			{
				oPFRun.ApplyAfter(m_oStyles.m_pLevels[lLevel]->m_oPFRun);
			}
			oPFRun.ApplyAfter(pParagraph->m_oPFRun);

			// теперь спаны
			size_t nSpans = pParagraph->m_arSpans.size();
			CString strCFs = _T("");
			for (size_t j = 0; j < nSpans; ++j)
			{
				CSpan* pSpan = &pParagraph->m_arSpans[j];
				CString strClassCF = _T("");

				CTextCFRun oCFRun;
				if (-1 != m_lTextType)
				{
					CString strClassTheme = _T("");
					strClassTheme.Format(_T("s%d_lvl%d_cf"), m_lTextType, lLevel);
					strClassCF += strClassTheme;
				}
				if (-1 != m_lPlaceholderType)
				{
					LONG lCountThisType = 0;
					if (NULL != pLayout)
						lCountThisType = pLayout->GetCountPlaceholderWithType(m_lPlaceholderType);
					
					if (1 < lCountThisType)
					{
						CString strPlaceholderStyle = _T("");
						strPlaceholderStyle.Format(_T("l_font%d t%d_lvl%d_id%d_cf"), m_lPlaceholderType, m_lPlaceholderType, lLevel, m_lPlaceholderID);

						strClassCF += _T(" ");
						strClassCF += strPlaceholderStyle;
					}
					else
					{
						CString strPlaceholderStyle = _T("");
						strPlaceholderStyle.Format(_T("l_font%d t%d_lvl%d_cf"), m_lPlaceholderType, m_lPlaceholderType, lLevel);

						strClassCF += _T(" ");
						strClassCF += strPlaceholderStyle;
					}
				}
				if (m_bIsSlideFontRef && -1 != m_lFontRef)
				{
					CString strClassRef = _T("");
					strClassRef.Format(_T(" s_font%d"), oElemInfo.m_lID);
					strClassCF += strClassRef;
				}
				if (m_oStyles.m_pLevels[lLevel].is_init())
				{
					oCFRun.ApplyAfter(m_oStyles.m_pLevels[lLevel]->m_oCFRun);
				}
				oCFRun.ApplyAfter(pSpan->m_oRun);

				CString strRunText = pSpan->m_strText;
				if (oCFRun.Cap.is_init())
				{
					if (1 == oCFRun.Cap.get())
						strRunText.MakeUpper();
					else if (2 == oCFRun.Cap.get())
						strRunText.MakeLower();
				}
				NormalizeString(strRunText);

				strCFs += oCFRun.ToXmlEditor(strClassCF, strRunText);
			}

			strHTML += oPFRun.ToXmlEditor(strClass, strCFs, oInfo);
		}

		strHTML += _T("</body>");
		strHTML += _T("</text>");

		return strHTML;
	}

	void CTextAttributesEx::RecalcParagraphs(CTheme* pTheme)
	{
		#ifdef PPT_FORMAT
		for (LONG i = 0; i < (LONG)m_arParagraphs.size(); ++i)
		{
			bool bIsBreak = true;
			LONG lCountCFs = (LONG)m_arParagraphs[i].m_arSpans.size();
			for (LONG j = 0; j < lCountCFs; ++j)
			{
				CString s = m_arParagraphs[i].m_arSpans[j].m_strText;
				LONG lFound = m_arParagraphs[i].m_arSpans[j].m_strText.Find((TCHAR)13);
				if (-1 != lFound)
				{
					bool bIsBreakAttack = false;
					if (bIsBreak && (0 == lFound))
						bIsBreakAttack = true;

					if (bIsBreakAttack)
					{
						CParagraph oParBreak = m_arParagraphs[i];
						oParBreak.m_arSpans.clear();
						oParBreak.m_arSpans.push_back(m_arParagraphs[i].m_arSpans[j]);
						oParBreak.m_arSpans[0].m_strText = _T("  ");
					}

					bIsBreak = true;

					// разбиваем параграф
					CParagraph oNewPar = m_arParagraphs[i];

					if (lCountCFs - j - 1 > 0)
					{
						m_arParagraphs[i].m_arSpans.RemoveAt(j + 1, lCountCFs - j - 1);
					}
					
					LONG lCountTx = m_arParagraphs[i].m_arSpans[j].m_strText.GetLength();
					m_arParagraphs[i].m_arSpans[j].m_strText.Delete(lFound, lCountTx - lFound);

					if (j > 0)
						oNewPar.m_arSpans.RemoveAt(0, j);

					if (lFound == (lCountTx - 1))
						oNewPar.m_arSpans.RemoveAt(0);
					else
						oNewPar.m_arSpans[0].m_strText.Delete(0, lFound + 1);

					if (0 != oNewPar.m_arSpans.size())
						m_arParagraphs.InsertAt(i + 1, oNewPar);	

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
				nullable_base<BOOL>		hasBullet;
				nullable_base<CColor>	bulletColor;
				nullable_base<LONG>		indent;
				nullable_base<LONG>		margin;
				if (-1 != m_lTextType && pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel].is_init())
				{
					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
						hasBullet = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.hasBullet;
					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletColor.is_init())
						bulletColor = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.bulletColor;
					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.leftMargin;
					if (pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = pTheme->m_pStyles[m_lTextType].m_pLevels[lLevel]->m_oPFRun.indent;
				}
				if (m_oLayoutStyles.m_pLevels[lLevel].is_init())
				{
					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
						hasBullet = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet;
					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor.is_init())
						bulletColor = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor;
					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin;
					if (m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = m_oLayoutStyles.m_pLevels[lLevel]->m_oPFRun.indent;
				}
				if (m_oStyles.m_pLevels[lLevel].is_init())
				{
					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet.is_init())
						hasBullet = m_oStyles.m_pLevels[lLevel]->m_oPFRun.hasBullet;
					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor.is_init())
						bulletColor = m_oStyles.m_pLevels[lLevel]->m_oPFRun.bulletColor;
					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin.is_init())
						margin = m_oStyles.m_pLevels[lLevel]->m_oPFRun.leftMargin;
					if (m_oStyles.m_pLevels[lLevel]->m_oPFRun.indent.is_init())
						indent = m_oStyles.m_pLevels[lLevel]->m_oPFRun.indent;
				}
				
				if (!m_arParagraphs[nIndexP].m_oPFRun.hasBullet.is_init())
					m_arParagraphs[nIndexP].m_oPFRun.hasBullet = hasBullet;
				if (!m_arParagraphs[nIndexP].m_oPFRun.bulletColor.is_init())
					m_arParagraphs[nIndexP].m_oPFRun.bulletColor = bulletColor;
				if (!m_arParagraphs[nIndexP].m_oPFRun.leftMargin.is_init())
					m_arParagraphs[nIndexP].m_oPFRun.leftMargin = margin;
				if (!m_arParagraphs[nIndexP].m_oPFRun.indent.is_init())
					m_arParagraphs[nIndexP].m_oPFRun.indent = indent;
			}
		}

		#endif

		size_t nCount = m_arParagraphs.size();
		for (size_t i = 0; i < nCount; ++i)
		{
			m_arParagraphs[i].CheckErrors();
		}
	}
}
