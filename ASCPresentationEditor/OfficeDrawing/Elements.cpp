
#include "Slide.h"

void NSPresentationEditor::CShapeElement::CalculateColor(CColor& oColor, CSlide* pSlide, CTheme* pTheme, CLayout* pLayout)
{
	LONG lOldIndex = oColor.m_lSchemeIndex;
	if (-1 == oColor.m_lSchemeIndex)
		return;

    std::vector<CColor>* pArray = NULL;

    if (pTheme) pArray = &pTheme->m_arColorScheme;

    if ((NULL != pLayout) && (!pLayout->m_bUseThemeColorScheme))
		pArray = &pLayout->m_arColorScheme;

	if (NULL != pSlide)
	{
		if (!pSlide->m_bUseLayoutColorScheme)
			pArray = &pSlide->m_arColorScheme;
	}

    if ((0 > oColor.m_lSchemeIndex) || ((pArray) && (oColor.m_lSchemeIndex >= (LONG)pArray->size())))
		return;

    if (pArray)
    {
        oColor = pArray->at(oColor.m_lSchemeIndex);
    }
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
			if (NULL != pTheme)
			{
				if ((pRun->Typeface.is_init()) && (pRun->Typeface.get() < pTheme->m_arFonts.size()))
				{
					pRun->FontProperties = new CFontProperties();
					pRun->FontProperties->SetFont(pTheme->m_arFonts[pRun->Typeface.get()]);

					if (1 < pRun->Typeface.get())
						pRun->Typeface.reset();
				}
				if ((pRun->EAFontRef.is_init()) && (pRun->EAFontRef.get() < pTheme->m_arFonts.size()))
				{
					pRun->FontPropertiesEA = new CFontProperties();
					pRun->FontPropertiesEA->SetFont(pTheme->m_arFonts[pRun->EAFontRef.get()]);
				}
				if ((pRun->SymbolFontRef.is_init()) && (pRun->SymbolFontRef.get() < pTheme->m_arFonts.size()))
				{
					pRun->FontPropertiesSym = new CFontProperties();
					pRun->FontPropertiesSym->SetFont(pTheme->m_arFonts[pRun->SymbolFontRef.get()]);
				}
			}
		}
	}
}

void NSPresentationEditor::CShapeElement::SetUpText(std::wstring newText)
{
	NSPresentationEditor::CTextAttributesEx* pText = &m_oShape.m_oText;

	if (pText->m_arParagraphs.size() > 0)
	{
		if (pText->m_arParagraphs[0].m_arSpans.size() >0)
		{
			ReplaceAll(pText->m_arParagraphs[0].m_arSpans[0].m_strText, L"*", newText);
			pText->m_arParagraphs[0].m_arSpans.erase(pText->m_arParagraphs[0].m_arSpans.begin()+1,pText->m_arParagraphs[0].m_arSpans.end());
		}
		pText->m_arParagraphs.erase(pText->m_arParagraphs.begin()+1,pText->m_arParagraphs.end());


	}

}