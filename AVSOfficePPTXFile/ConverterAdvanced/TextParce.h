#pragma once
#include "Structures.h"

#include "../PPTXFormat/Logic/Paragraph.h"
#include "../PPTXFormat/Logic/RunElem.h"
#include "../PPTXFormat/Logic/Runs/Run.h"
#include "../PPTXFormat/Logic/Runs/Fld.h"
#include "../PPTXFormat/Logic/Runs/Br.h"
#include "../PPTXFormat/Logic/RunProperties.h"

#include "TextStyleConvert.h"

namespace PPTX2EditorAdvanced
{
	AVSINLINE BYTE GetTextStrike(const CString& str)
	{
		if (str == _T("noStrike"))	return 0;
		if (str == _T("sngStrike")) return 1;
		if (str == _T("dblStrike")) return 2;
		return 0;
	}

	void TextParse(const PPTX::Logic::TxBody& pShapeText, NSPresentationEditor::CTextAttributesEx& pText, const PPTX::Logic::ShapeProperties& TextProps, double FontScale, double LnSpcReduction, NSPresentationEditor::CTheme* pTheme)
	{		
		pText.m_oAttributes.m_nTextAlignVertical = GetTextAnchorFromStr(TextProps.GetAnchor());
		//pText.m_bVertical = (pShapeText.bodyPr->vert.get_value_or("horz") != "horz");
		//pText.m_oAttributes.m_dTextRotate = pShapeText.bodyPr->rot.get_value_or(0) / 60000.0;
		pText.m_lTextType = (DWORD)TextProps.GetTextType();

		pText.m_lFontRef = -1;

		size_t nCount = pShapeText.Paragrs.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			const PPTX::Logic::Paragraph* SpTextPar = &pShapeText.Paragrs[i];
			
			CParagraph oParagraph;
			oParagraph.m_oPFRun.bIsOneLine = pShapeText.IsOneLineParagraphs();

			int level = 0;

			if (SpTextPar->pPr.is_init())
				level = SpTextPar->pPr->lvl.get_value_or(0);

			oParagraph.m_lTextType	= (LONG)TextProps.GetTextType();
			oParagraph.m_lTextLevel	= level;

			bool bIsBullet = TextProps.HasParagraphBullet(level, SpTextPar->pPr);

			oParagraph.m_oPFRun.hasBullet = (BOOL)bIsBullet;
			if (bIsBullet)
			{
				oParagraph.m_oPFRun.bulletColor = new CColor();
				
				PPTX::Logic::UniColor oBulletColor = TextProps.GetColorBullet(level, SpTextPar->pPr);

				oParagraph.m_oPFRun.bulletColor.get() = oBulletColor.GetABGR();
				if (oBulletColor.is<PPTX::Logic::SchemeClr>())
				{
					oParagraph.m_oPFRun.bulletColor->m_lSchemeIndex = GetColorIndexFromStr(oBulletColor.as<PPTX::Logic::SchemeClr>().val.get());
				}

				oParagraph.m_oPFRun.bulletSize = 100;
				oParagraph.m_oPFRun.bulletChar = (WCHAR)'*';
			}
			
			oParagraph.m_oPFRun.textAlignment	= (WORD)GetTextAlignFromStr ( TextProps.GetParagraphAlgn ( level, SpTextPar->pPr) );
			oParagraph.m_oPFRun.leftMargin		= (LONG)TextProps.GetParagraphLeftMargin(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.indent			= (LONG)TextProps.GetParagraphIndent(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.defaultTabSize	= (LONG)TextProps.GetParagraphDefTabSz(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.fontAlign		= (WORD)GetFontAlignFromStr(TextProps.GetParagraphFontAlgn(level, SpTextPar->pPr));

			WORD dwWrap = 0;
			dwWrap = 0x02;
			if (TextProps.GetParagraphLatinLnBrk(level, SpTextPar->pPr))
			{
				dwWrap = 0x01;
			}
			oParagraph.m_oPFRun.wrapFlags = (WORD)dwWrap;

			oParagraph.m_oPFRun.textDirection	= (TextProps.GetParagraphRtl(level, SpTextPar->pPr)) ? 1 : 0;

			int space = TextProps.GetParagraphLnSpc(level, SpTextPar->pPr);
			if(space < 0)
				oParagraph.m_oPFRun.lineSpacing = (LONG)(space * 127 * (1 - LnSpcReduction));// - 13200;
			else
				oParagraph.m_oPFRun.lineSpacing = (LONG)(space * (1 - LnSpcReduction));

			space = TextProps.GetParagraphSpcBef(level, SpTextPar->pPr);
			if(space < 0)
				oParagraph.m_oPFRun.spaceBefore = (LONG)(space * 127);// - 13200;
			else
				oParagraph.m_oPFRun.spaceBefore = (LONG)space;

			space = TextProps.GetParagraphSpcAft(level, SpTextPar->pPr);
			if(space < 0)
				oParagraph.m_oPFRun.spaceAfter	= (LONG)(space * 127);// - 13200;
			else
				oParagraph.m_oPFRun.spaceAfter	= (LONG)space;

			size_t nCountCF = SpTextPar->RunElems.GetCount();
			for (size_t j = 0; j < nCountCF; ++j)
			{
				const PPTX::Logic::RunElem* SpTextRun = &SpTextPar->RunElems[j];

				CSpan oSpan;
				oSpan.m_strText = SpTextRun->GetText();
				
				if (SpTextRun->is<PPTX::Logic::Run>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Run>().rPr;
					
					oSpan.m_oRun.FontBold		= (BOOL)TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontItalic		= (BOOL)TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontUnderline	= (BOOL)((TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != _T("none")) ? TRUE : FALSE);
					oSpan.m_oRun.FontStrikeout	= (BOOL)GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));
					
					bool isHyperlink = false;
					if (lpSpTextRun.is_init())
					{
						if (lpSpTextRun->hlinkClick.is_init())
						{
							oSpan.m_oRun.FontUnderline = TRUE;
							isHyperlink = true;
						}
					}

					oSpan.m_oRun.Size			= (WORD)((TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50) / 100);

					oSpan.m_oRun.FontProperties = new CFontProperties();
					LONG lFontIndex = -1;
					oSpan.m_oRun.FontProperties->strFontName	= TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr, lFontIndex);
					oSpan.m_oRun.FontProperties->strPanose		= TextProps.GetRunPanose(level, lpSpTextRun, SpTextPar->pPr);

					if (-1 != lFontIndex)
						oSpan.m_oRun.Typeface = (WORD)lFontIndex;
					
					oSpan.m_oRun.FontProperties->arFontCharsets.RemoveAll();
					oSpan.m_oRun.FontProperties->arFontCharsets.Add(TextProps.GetRunCharset(level, lpSpTextRun, SpTextPar->pPr));
					
					BYTE fontPitchFamily = TextProps.GetRunPitchFamily(level, lpSpTextRun, SpTextPar->pPr);
					switch(fontPitchFamily % 0x10)
					{
					case 0: {oSpan.m_oRun.FontProperties->lFontFixed = -1; break;}
					case 1: {oSpan.m_oRun.FontProperties->lFontFixed = 1; break;}
					case 2: {oSpan.m_oRun.FontProperties->lFontFixed = 0; break;}
					default: {oSpan.m_oRun.FontProperties->lFontFixed = -1; break;}
					}
					switch(fontPitchFamily / 0x10)
					{
					case 0: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("unknown"); break;}
					case 1: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("roman"); break;}
					case 2: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("swiss"); break;}
					case 3: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("modern"); break;}
					case 4: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("script"); break;}
					case 5: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("decorative"); break;}
					default: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("unknown"); break;}
					}
					if (isHyperlink)
					{
						oSpan.m_oRun.Color = new CColor();
						oSpan.m_oRun.Color.get() = TextProps.GetHyperlinkBGRA();
						oSpan.m_oRun.Color->m_lSchemeIndex = 11;
					}
					else
					{
						PPTX::Logic::UniColor oUniColor = TextProps.GetColor(level, lpSpTextRun, SpTextPar->pPr);
						
						if (oUniColor.is_init())
						{
							oSpan.m_oRun.Color = new CColor();
							oSpan.m_oRun.Color.get() = oUniColor.GetBGRA();
							
							if(oUniColor.is<PPTX::Logic::SchemeClr>())
							{
								oSpan.m_oRun.Color->m_lSchemeIndex = GetColorIndexFromStr(oUniColor.as<PPTX::Logic::SchemeClr>().val.get());
							}
						}
					}

					oSpan.m_oRun.BaseLineOffset = TextProps.GetRunBaseline(level, lpSpTextRun, SpTextPar->pPr) / 1000;
					CString strCap				= TextProps.GetRunCap(level, lpSpTextRun, SpTextPar->pPr);
					if (_T("all") == strCap)
						oSpan.m_oRun.Cap = (WORD)1;
					else if (_T("small") == strCap)
						oSpan.m_oRun.Cap = (WORD)2;
					else
						oSpan.m_oRun.Cap = (WORD)0;
				}
				else if(SpTextRun->is<PPTX::Logic::Fld>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Fld>().rPr;
					
					CSpan oSpan;
					oSpan.m_strText = SpTextRun->GetText();

					oSpan.m_oRun.FontBold		= (BOOL)TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontItalic		= (BOOL)TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontUnderline	= (BOOL)((TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != _T("none")) ? TRUE : FALSE);
					oSpan.m_oRun.FontStrikeout	= (BOOL)GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));

					bool isHyperlink = false;
					if (lpSpTextRun.is_init())
					{
						if (lpSpTextRun->hlinkClick.is_init())
						{
							oSpan.m_oRun.FontUnderline = TRUE;
							isHyperlink = true;
						}
					}

					oSpan.m_oRun.Size			= (WORD)((TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50) / 100);

					oSpan.m_oRun.FontProperties->arFontCharsets.RemoveAll();
					oSpan.m_oRun.FontProperties->arFontCharsets.Add(TextProps.GetRunCharset(level, lpSpTextRun, SpTextPar->pPr));
					
					BYTE fontPitchFamily = TextProps.GetRunPitchFamily(level, lpSpTextRun, SpTextPar->pPr);
					switch(fontPitchFamily % 0x10)
					{
					case 0: {oSpan.m_oRun.FontProperties->lFontFixed = -1; break;}
					case 1: {oSpan.m_oRun.FontProperties->lFontFixed = 1; break;}
					case 2: {oSpan.m_oRun.FontProperties->lFontFixed = 0; break;}
					default: {oSpan.m_oRun.FontProperties->lFontFixed = -1; break;}
					}
					switch(fontPitchFamily / 0x10)
					{
					case 0: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("unknown"); break;}
					case 1: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("roman"); break;}
					case 2: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("swiss"); break;}
					case 3: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("modern"); break;}
					case 4: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("script"); break;}
					case 5: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("decorative"); break;}
					default: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("unknown"); break;}
					}
					if (isHyperlink)
					{
						oSpan.m_oRun.Color = new CColor();
						oSpan.m_oRun.Color.get() = TextProps.GetHyperlinkBGRA();
						oSpan.m_oRun.Color->m_lSchemeIndex = 11;
					}
					else
					{
						PPTX::Logic::UniColor oUniColor = TextProps.GetColor(level, lpSpTextRun, SpTextPar->pPr);
						
						if (oUniColor.is_init())
						{
							oSpan.m_oRun.Color = new CColor();
							oSpan.m_oRun.Color.get() = oUniColor.GetBGRA();
							
							if(oUniColor.is<PPTX::Logic::SchemeClr>())
							{
								oSpan.m_oRun.Color->m_lSchemeIndex = GetColorIndexFromStr(oUniColor.as<PPTX::Logic::SchemeClr>().val.get());
							}
						}
					}
				}
				else if(SpTextRun->is<PPTX::Logic::Br>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Br>().rPr;
					oSpan.m_oRun.Size = (WORD)((TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50) / 100);
				}
				if (oSpan.m_strText != _T(""))
				{
					oParagraph.m_arSpans.Add(oSpan);
				}
			}

			if (SpTextPar->endParaRPr.is_init())
			{
				CSpan oSpan;
				oSpan.m_strText = _T("\n");

				const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextPar->endParaRPr;
					
				oSpan.m_oRun.FontBold		= (BOOL)TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
				oSpan.m_oRun.FontItalic		= (BOOL)TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
				oSpan.m_oRun.FontUnderline	= (BOOL)((TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr) != _T("none")) ? TRUE : FALSE);
				oSpan.m_oRun.FontStrikeout	= (BOOL)GetTextStrike(TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr));
				
				bool isHyperlink = false;
				if (lpSpTextRun.is_init())
				{
					if (lpSpTextRun->hlinkClick.is_init())
					{
						oSpan.m_oRun.FontUnderline = TRUE;
						isHyperlink = true;
					}
				}

				oSpan.m_oRun.Size			= (WORD)((TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr) * FontScale + 50) / 100);

				oSpan.m_oRun.FontProperties = new CFontProperties();
				LONG lFontIndex = -1;
				oSpan.m_oRun.FontProperties->strFontName	= TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr, lFontIndex);
				oSpan.m_oRun.FontProperties->strPanose		= TextProps.GetRunPanose(level, lpSpTextRun, SpTextPar->pPr);

				if (-1 != lFontIndex)
					oSpan.m_oRun.Typeface = (WORD)lFontIndex;
				
				oSpan.m_oRun.FontProperties->arFontCharsets.RemoveAll();
				oSpan.m_oRun.FontProperties->arFontCharsets.Add(TextProps.GetRunCharset(level, lpSpTextRun, SpTextPar->pPr));
				
				BYTE fontPitchFamily = TextProps.GetRunPitchFamily(level, lpSpTextRun, SpTextPar->pPr);
				switch(fontPitchFamily % 0x10)
				{
				case 0: {oSpan.m_oRun.FontProperties->lFontFixed = -1; break;}
				case 1: {oSpan.m_oRun.FontProperties->lFontFixed = 1; break;}
				case 2: {oSpan.m_oRun.FontProperties->lFontFixed = 0; break;}
				default: {oSpan.m_oRun.FontProperties->lFontFixed = -1; break;}
				}
				switch(fontPitchFamily / 0x10)
				{
				case 0: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("unknown"); break;}
				case 1: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("roman"); break;}
				case 2: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("swiss"); break;}
				case 3: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("modern"); break;}
				case 4: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("script"); break;}
				case 5: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("decorative"); break;}
				default: {oSpan.m_oRun.FontProperties->strPitchFamily = _T("unknown"); break;}
				}
				if (isHyperlink)
				{
					oSpan.m_oRun.Color = new CColor();
					oSpan.m_oRun.Color.get() = TextProps.GetHyperlinkBGRA();
					oSpan.m_oRun.Color->m_lSchemeIndex = 11;
				}
				else
				{
					PPTX::Logic::UniColor oUniColor = TextProps.GetColor(level, lpSpTextRun, SpTextPar->pPr);
					
					if (oUniColor.is_init())
					{
						oSpan.m_oRun.Color = new CColor();
						oSpan.m_oRun.Color.get() = oUniColor.GetBGRA();
						
						if(oUniColor.is<PPTX::Logic::SchemeClr>())
						{
							oSpan.m_oRun.Color->m_lSchemeIndex = GetColorIndexFromStr(oUniColor.as<PPTX::Logic::SchemeClr>().val.get());
						}
					}
				}

				oSpan.m_oRun.BaseLineOffset = TextProps.GetRunBaseline(level, lpSpTextRun, SpTextPar->pPr) / 1000;
				CString strCap				= TextProps.GetRunCap(level, lpSpTextRun, SpTextPar->pPr);
				if (_T("all") == strCap)
					oSpan.m_oRun.Cap = (WORD)1;
				else if (_T("small") == strCap)
					oSpan.m_oRun.Cap = (WORD)2;
				else
					oSpan.m_oRun.Cap = (WORD)0;

				oParagraph.m_arSpans.Add(oSpan);
			}

			pText.m_arParagraphs.Add(oParagraph);
		}
	}
	
	void TextParse(const PPTX::Logic::TxBody& pShapeText, NSPresentationEditor::CTextAttributesEx& pText, PPTX::Logic::CShapeTextProperties& TextProps, 
		double FontScale, double LnSpcReduction, NSPresentationEditor::CTheme* pTheme)
	{		
		pText.m_oAttributes.m_nTextAlignVertical = GetTextAnchorFromStr(TextProps.GetAnchor());
		//pText.m_bVertical = (pShapeText.bodyPr->vert.get_value_or("horz") != "horz");
		//pText.m_oAttributes.m_dTextRotate = pShapeText.bodyPr->rot.get_value_or(0) / 60000.0;
		pText.m_lTextType = (DWORD)TextProps.GetTextType();

		pText.m_lFontRef = TextProps.GetFontRef(pText.m_bIsSlideFontRef);

		nullable<PPTX::Logic::TextParagraphPr>* pLevels = TextProps.GetLevels();
		nullable<PPTX::Logic::TextParagraphPr>* pBody	= TextProps.GetLevelsBody();

		if (NULL != pLevels)
			TextStyleConvert(pLevels, pText.m_oLayoutStyles, pTheme->m_arFonts);
		TextStyleConvert(pBody, pText.m_oStyles, pTheme->m_arFonts);

		size_t nCount = pShapeText.Paragrs.GetCount();
		for (size_t i = 0; i < nCount; ++i)
		{
			const PPTX::Logic::Paragraph* SpTextPar = &pShapeText.Paragrs[i];
			
			CParagraph oParagraph;
			oParagraph.m_oPFRun.bIsOneLine = pShapeText.IsOneLineParagraphs();

			int level = 0;

			if(SpTextPar->pPr.is_init())
				level = /*(*/SpTextPar->pPr->lvl.get_value_or(0);//>4) ?4:(SpTextPar->pPr->lvl.is_init() ?SpTextPar->pPr->lvl.get():0);

			oParagraph.m_lTextType	= TextProps.GetTextType();
			oParagraph.m_lTextLevel	= level;

			nullable_base<BOOL> bIsHasBullet = TextProps.GetHasBullet(level, SpTextPar->pPr);
			if (bIsHasBullet.is_init())
				oParagraph.m_oPFRun.hasBullet = bIsHasBullet;
			else
			{
				if (pTheme->m_pStyles[oParagraph.m_lTextType].m_pLevels[level].is_init())
				{
					oParagraph.m_oPFRun.hasBullet = pTheme->m_pStyles[oParagraph.m_lTextType].m_pLevels[level]->m_oPFRun.hasBullet;
				}
			}

			PPTX::Logic::UniColor oBulletColor = TextProps.GetColorBullet(level, SpTextPar->pPr);
			if (oBulletColor.is_init())
			{				
				oParagraph.m_oPFRun.bulletColor = new CColor();
				oParagraph.m_oPFRun.bulletColor.get() = oBulletColor.GetABGR();
				if (oBulletColor.is<PPTX::Logic::SchemeClr>())
				{
					oParagraph.m_oPFRun.bulletColor->m_lSchemeIndex = GetColorIndexFromStr(oBulletColor.as<PPTX::Logic::SchemeClr>().val.get());
				}
			}

			oParagraph.m_oPFRun.textAlignment	= TextProps.GetParagraphAlgn(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.leftMargin		= TextProps.GetParagraphLeftMargin(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.indent			= TextProps.GetParagraphIndent(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.defaultTabSize	= TextProps.GetParagraphDefTabSz(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.fontAlign		= TextProps.GetParagraphFontAlgn(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.wrapFlags		= TextProps.GetParagraphLatinLnBrk(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.textDirection	= TextProps.GetParagraphRtl(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.lineSpacing		= TextProps.GetParagraphLnSpc(level, SpTextPar->pPr, LnSpcReduction);
			oParagraph.m_oPFRun.spaceBefore		= TextProps.GetParagraphSpcBef(level, SpTextPar->pPr);
			oParagraph.m_oPFRun.spaceAfter		= TextProps.GetParagraphSpcAft(level, SpTextPar->pPr);

			size_t nCountCF = SpTextPar->RunElems.GetCount();
			for (size_t j = 0; j < nCountCF; ++j)
			{
				const PPTX::Logic::RunElem* SpTextRun = &SpTextPar->RunElems[j];

				CSpan oSpan;
				oSpan.m_strText = SpTextRun->GetText();
				
				if (SpTextRun->is<PPTX::Logic::Run>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Run>().rPr;
					
					oSpan.m_oRun.FontBold		= TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontItalic		= TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontUnderline	= TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontStrikeout	= TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr);
					
					bool isHyperlink = false;
					if (lpSpTextRun.is_init())
					{
						if (lpSpTextRun->hlinkClick.is_init())
						{
							oSpan.m_oRun.FontUnderline = TRUE;
							isHyperlink = true;
						}
					}

					oSpan.m_oRun.Size			= TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr, FontScale);

					nullable_base<PPTX::Logic::CFontInfo>	propInfo = TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr);

					if (propInfo.is_init())
					{
						oSpan.m_oRun.FontProperties = new CFontProperties();
						oSpan.m_oRun.FontProperties->strFontName	= propInfo->strFontName;
						oSpan.m_oRun.FontProperties->strPanose		= propInfo->strPanose;
						oSpan.m_oRun.FontProperties->strPitchFamily	= propInfo->strPitchFamily;
						oSpan.m_oRun.FontProperties->arFontCharsets.Add(propInfo->Charset);
						
						if (propInfo->FontRef != -1)
						{
							oSpan.m_oRun.Typeface = (WORD)propInfo->FontRef;
						}
					}

					if (isHyperlink)
					{
						oSpan.m_oRun.Color = new CColor();
						oSpan.m_oRun.Color.get() = TextProps.GetHyperlinkBGRA();
						oSpan.m_oRun.Color->m_lSchemeIndex = 11;
					}
					else
					{
						PPTX::Logic::UniColor oUniColor = TextProps.GetColor(level, lpSpTextRun, SpTextPar->pPr);
						
						if (oUniColor.is_init())
						{
							oSpan.m_oRun.Color = new CColor();
							oSpan.m_oRun.Color.get() = oUniColor.GetBGRA();
							
							if(oUniColor.is<PPTX::Logic::SchemeClr>())
							{
								oSpan.m_oRun.Color->m_lSchemeIndex = GetColorIndexFromStr(oUniColor.as<PPTX::Logic::SchemeClr>().val.get());
							}
						}
					}

					oSpan.m_oRun.BaseLineOffset = TextProps.GetRunBaseline(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.Cap			= TextProps.GetRunCap(level, lpSpTextRun, SpTextPar->pPr);
				}
				else if(SpTextRun->is<PPTX::Logic::Fld>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Fld>().rPr;
					
					CSpan oSpan;
					oSpan.m_strText = SpTextRun->GetText();

					oSpan.m_oRun.FontBold		= TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontItalic		= TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontUnderline	= TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
					oSpan.m_oRun.FontStrikeout	= TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr);

					oSpan.m_oRun.Size			= TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr, FontScale);

					nullable_base<PPTX::Logic::CFontInfo>	propInfo = TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr);

					if (propInfo.is_init())
					{
						oSpan.m_oRun.FontProperties = new CFontProperties();
						oSpan.m_oRun.FontProperties->strFontName	= propInfo->strFontName;
						oSpan.m_oRun.FontProperties->strPanose		= propInfo->strPanose;
						oSpan.m_oRun.FontProperties->strPitchFamily	= propInfo->strPitchFamily;
						oSpan.m_oRun.FontProperties->arFontCharsets.Add(propInfo->Charset);
						
						if (propInfo->FontRef != -1)
						{
							oSpan.m_oRun.Typeface = (WORD)propInfo->FontRef;
						}
					}

					PPTX::Logic::UniColor oUniColor = TextProps.GetColor(level, lpSpTextRun, SpTextPar->pPr);
					if (oUniColor.is_init())
					{
						oSpan.m_oRun.Color = new CColor();
						oSpan.m_oRun.Color.get() = oUniColor.GetBGRA();
						
						if(oUniColor.is<PPTX::Logic::SchemeClr>())
						{
							oSpan.m_oRun.Color->m_lSchemeIndex = GetColorIndexFromStr(oUniColor.as<PPTX::Logic::SchemeClr>().val.get());
						}
					}
				}
				else if(SpTextRun->is<PPTX::Logic::Br>())
				{
					const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextRun->as<PPTX::Logic::Br>().rPr;

					oSpan.m_oRun.Size = TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr, FontScale);
				}
				if (oSpan.m_strText != _T(""))
				{
					oParagraph.m_arSpans.Add(oSpan);
				}
			}

			if (SpTextPar->endParaRPr.is_init())
			{
				CSpan oSpan;
				oSpan.m_strText = _T("\n");

				const nullable<PPTX::Logic::RunProperties>& lpSpTextRun = SpTextPar->endParaRPr;
					
				oSpan.m_oRun.FontBold		= TextProps.GetRunBold(level, lpSpTextRun, SpTextPar->pPr);
				oSpan.m_oRun.FontItalic		= TextProps.GetRunItalic(level, lpSpTextRun, SpTextPar->pPr);
				oSpan.m_oRun.FontUnderline	= TextProps.GetRunUnderline(level, lpSpTextRun, SpTextPar->pPr);
				oSpan.m_oRun.FontStrikeout	= TextProps.GetRunStrike(level, lpSpTextRun, SpTextPar->pPr);
				
				bool isHyperlink = false;
				if (lpSpTextRun.is_init())
				{
					if (lpSpTextRun->hlinkClick.is_init())
					{
						oSpan.m_oRun.FontUnderline = TRUE;
						isHyperlink = true;
					}
				}

				oSpan.m_oRun.Size			= TextProps.GetRunSize(level, lpSpTextRun, SpTextPar->pPr, FontScale);

				nullable_base<PPTX::Logic::CFontInfo>	propInfo = TextProps.GetRunFont(level, lpSpTextRun, SpTextPar->pPr);

				if (propInfo.is_init())
				{
					oSpan.m_oRun.FontProperties = new CFontProperties();
					oSpan.m_oRun.FontProperties->strFontName	= propInfo->strFontName;
					oSpan.m_oRun.FontProperties->strPanose		= propInfo->strPanose;
					oSpan.m_oRun.FontProperties->strPitchFamily	= propInfo->strPitchFamily;
					oSpan.m_oRun.FontProperties->arFontCharsets.Add(propInfo->Charset);
					
					if (propInfo->FontRef != -1)
					{
						oSpan.m_oRun.Typeface = (WORD)propInfo->FontRef;
					}
				}

				if (isHyperlink)
				{
					oSpan.m_oRun.Color = new CColor();
					oSpan.m_oRun.Color.get() = TextProps.GetHyperlinkBGRA();
					oSpan.m_oRun.Color->m_lSchemeIndex = 11;
				}
				else
				{
					PPTX::Logic::UniColor oUniColor = TextProps.GetColor(level, lpSpTextRun, SpTextPar->pPr);
					
					if (oUniColor.is_init())
					{
						oSpan.m_oRun.Color = new CColor();
						oSpan.m_oRun.Color.get() = oUniColor.GetBGRA();
						
						if(oUniColor.is<PPTX::Logic::SchemeClr>())
						{
							oSpan.m_oRun.Color->m_lSchemeIndex = GetColorIndexFromStr(oUniColor.as<PPTX::Logic::SchemeClr>().val.get());
						}
					}
				}

				oSpan.m_oRun.BaseLineOffset = TextProps.GetRunBaseline(level, lpSpTextRun, SpTextPar->pPr);
				oSpan.m_oRun.Cap			= TextProps.GetRunCap(level, lpSpTextRun, SpTextPar->pPr);

				oParagraph.m_arSpans.Add(oSpan);
			}

			pText.m_arParagraphs.Add(oParagraph);
		}
	}
}