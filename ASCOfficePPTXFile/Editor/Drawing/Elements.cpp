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

bool NSPresentationEditor::CShapeElement::SetUpTextPlaceholder(std::wstring newText)
{
	bool result = false;
	NSPresentationEditor::CTextAttributesEx* pText = &m_oShape.m_oText;

	for (int p = 0 ; p < pText->m_arParagraphs.size(); p++) //тут по всем -> 1-(33).ppt
	{
		if (pText->m_arParagraphs[p].m_arSpans.size() >0)//??? по всем?
		{
			int pos = pText->m_arParagraphs[p].m_arSpans[0].m_strText.find(L"*");
			
			if (pos >= 0)
			{
				CSpan first = pText->m_arParagraphs[p].m_arSpans[0];
				CSpan last = pText->m_arParagraphs[p].m_arSpans[0];
				
				first.m_strText	= pText->m_arParagraphs[p].m_arSpans[0].m_strText.substr(0, pos);
				last.m_strText	= pText->m_arParagraphs[p].m_arSpans[0].m_strText.substr(pos + 1);

				pText->m_arParagraphs[p].m_arSpans[0].m_strText = newText;
				pText->m_arParagraphs[p].m_arSpans[0].m_bField	= true;

				if (last.m_strText.empty() == false)
					pText->m_arParagraphs[p].m_arSpans.insert(pText->m_arParagraphs[p].m_arSpans.begin() + 1, last);
				if (first.m_strText.empty() == false)
					pText->m_arParagraphs[p].m_arSpans.insert(pText->m_arParagraphs[p].m_arSpans.begin(), first);

				result = true;
			}
		}
	}
	return result;
}