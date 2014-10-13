//#include "stdafx.h"
#include "Slide.h"

void NSPresentationEditor::CShapeElement::CalculateColor(CColor& oColor, CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
{
	LONG lOldIndex = oColor.m_lSchemeIndex;
	if (-1 == oColor.m_lSchemeIndex)
		return;

	std::vector<CColor>* pArray = &pTheme->m_arColorScheme;
	if ((NULL != pLayout) && (!pLayout->m_bUseThemeColorScheme))
		pArray = &pLayout->m_arColorScheme;

	if (NULL != pSlide)
	{
		if (!pSlide->m_bUseLayoutColorScheme)
			pArray = &pSlide->m_arColorScheme;
	}

	if ((0 > oColor.m_lSchemeIndex) || (oColor.m_lSchemeIndex >= (LONG)pArray->GetCount()))
		return;

	oColor = pArray->GetAt(oColor.m_lSchemeIndex);
	oColor.m_lSchemeIndex = lOldIndex;
}

void NSPresentationEditor::CShapeElement::SetupTextProperties(CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
{
	NSPresentationEditor::CTextAttributesEx* pAttributes = &m_oShape.m_oText;
	int nCountColors = 0;
	if (NULL != pTheme)
		nCountColors = (int)pTheme->m_arColorScheme.size();

	size_t nCount = pAttributes->m_arParagraphs.size();
	for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
	{
		if (pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor.is_init())
		{
			int nColorIndex = (int)pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor->m_lSchemeIndex;
			if (0 <= nColorIndex && nColorIndex < nCountColors)
			{
				CalculateColor(pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor.get(), pSlide, pTheme, pLayout);
				pAttributes->m_arParagraphs[nIndex].m_oPFRun.bulletColor->m_lSchemeIndex = nColorIndex;
			}
		}

		size_t nCountCFs = pAttributes->m_arParagraphs[nIndex].m_arSpans.size();
		for (size_t i = 0; i < nCountCFs; ++i)
		{
			CTextCFRun* pRun = &pAttributes->m_arParagraphs[nIndex].m_arSpans[i].m_oRun;
			
			if (pRun->Color.is_init())
			{
				int nColorIndex = (int)pRun->Color->m_lSchemeIndex;
				if (0 <= nColorIndex && nColorIndex < nCountColors)
				{
					CalculateColor(pRun->Color.get(), pSlide, pTheme, pLayout);
					pRun->Color->m_lSchemeIndex = nColorIndex;
				}
			}
			
			if (pRun->Typeface.is_init() && (NULL != pTheme))
			{
				if (pRun->Typeface.get() < pTheme->m_arFonts.size())
				{
					CFont* pFont = &(pTheme->m_arFonts[pRun->Typeface.get()]);

					pRun->FontProperties = new CFontProperties();
										
					// font name
					pRun->FontProperties->strFontName = pFont->Name;

					// charset
					pRun->FontProperties->arFontCharsets.clear();
					pRun->FontProperties->arFontCharsets.push_back(pFont->Charset);

					// pitchfamily
					pRun->FontProperties->strPitchFamily = pFont->PitchFamily;

					// font panose
					pRun->FontProperties->strPanose		= pFont->Panose;
					// font fixed
					pRun->FontProperties->lFontFixed	= -1;

					if (1 < pRun->Typeface.get())
						pRun->Typeface.reset();
				}
				/*if (m_arCFs[nIndex].fontEAFontRef < pContainer->m_pFonts->size())
				{
					m_arCFs[nIndex].strFontName = (*(pContainer->m_pFonts))[m_arCFs[nIndex].fontEAFontRef].m_strFontName;
				}*/
			}
		}
	}
}
