#pragma once
#include "../stdafx.h"

#include "Structures.h"
#include "../PPTXFormat/Logic/Tab.h"

namespace PPTX2EditorAdvanced
{
	void RunPropertiesConvert(const PPTX::Logic::RunProperties& pRunProps, NSPresentationEditor::CTextCFRun& pCFRun, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		if (pRunProps.b.is_init())
		{
			if (pRunProps.b.get())
				pCFRun.FontBold = TRUE;
			else
				pCFRun.FontBold = FALSE;
		}
		if (pRunProps.i.is_init())
		{
			if (pRunProps.i.get())
				pCFRun.FontItalic = TRUE;
			else
				pCFRun.FontItalic = FALSE;
		}
		if (pRunProps.u.is_init())
		{
			if (_T("none") != pRunProps.u->get())
				pCFRun.FontUnderline = TRUE;
			else
				pCFRun.FontUnderline = FALSE;
		}
		if (pRunProps.strike.is_init())
		{
			if (_T("noStrike") != pRunProps.strike->get())
				pCFRun.FontStrikeout = TRUE;
			else
				pCFRun.FontStrikeout = FALSE;
		}

		if (pRunProps.baseline.is_init())
		{
			pCFRun.BaseLineOffset = (double)pRunProps.baseline.get() / 1000.0;//(baseline / 100000.0) * CFRun.fontSize * (25.4 / 72);
		}
		
		if (pRunProps.latin.is_init())
		{
			CString strTypeface = pRunProps.latin->typeface;
			pCFRun.FontProperties = new CFontProperties();
			
			if (strTypeface == _T("+mj-lt"))
			{
				strTypeface = arFonts[0].Name;
				pCFRun.Typeface = (WORD)0;
			}
			else if (strTypeface == _T("+mn-lt"))
			{
				strTypeface = arFonts[1].Name;
				pCFRun.Typeface = (WORD)1;
			}
			else if (strTypeface == _T(""))
			{
				strTypeface = arFonts[1].Name;
				pCFRun.Typeface = (WORD)1;
			}
			else
			{
				pCFRun.FontProperties->strFontName	= strTypeface;
				if (pRunProps.latin->panose.is_init())
				{
					pCFRun.FontProperties->strPanose = pRunProps.latin->panose.get();
				}
			}
		}

		if (pRunProps.Fill.is_init())
		{
			PPTX::Logic::UniColor oColor = pRunProps.GetColor();
			pCFRun.Color = new NSPresentationEditor::CColor();

			pCFRun.Color.get() = oColor.GetBGRA();
			if(oColor.is<PPTX::Logic::SchemeClr>())
			{
				pCFRun.Color->m_lSchemeIndex = GetColorIndexFromStr(oColor.as<PPTX::Logic::SchemeClr>().val.get());
			}
		}

		if (pRunProps.cap.is_init())
		{
			CString str = pRunProps.cap->get();
			if (_T("all") == str)
				pCFRun.Cap = (WORD)1;
			else if (_T("small") == str)
				pCFRun.Cap = (WORD)2;
			else
				pCFRun.Cap = (WORD)0;
		}

		if (pRunProps.sz.is_init())
		{
			pCFRun.Size = (WORD)((pRunProps.sz.get() + 50) / 100);
		}

		return;
	}

	void RunPropertiesConvert(const nullable<PPTX::Logic::RunProperties>& pRunProps, NSPresentationEditor::CTextCFRun& pCFRun, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		if(pRunProps.is_init())
			RunPropertiesConvert(*pRunProps, pCFRun, arFonts);
	}

	void ParagraphPropertiesConvert(const PPTX::Logic::TextParagraphPr& pptxStyle, NSPresentationEditor::CTextPFRun& pStyleLevel, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		if (pptxStyle.lnSpc.is_init())
		{
			int space = pptxStyle.lnSpc->GetVal();
			if(space < 0)
				pStyleLevel.lineSpacing = (LONG)(space * 127);
			else
				pStyleLevel.lineSpacing = (LONG)space;
		}
		if (pptxStyle.spcBef.is_init())
		{
			int space = pptxStyle.spcBef->GetVal();
			if(space < 0)
				pStyleLevel.spaceBefore = (LONG)(space * 127);
			else
				pStyleLevel.spaceBefore = (LONG)space;
		}
		if (pptxStyle.spcAft.is_init())
		{
			int space = pptxStyle.spcAft->GetVal();
			if(space < 0)
				pStyleLevel.spaceAfter = (LONG)(space * 127);
			else
				pStyleLevel.spaceAfter = (LONG)space;
		}

		if (pptxStyle.defTabSz.is_init())
			pStyleLevel.defaultTabSize = (LONG)(pptxStyle.defTabSz.get_value_or(376300));

		if (pptxStyle.marL.is_init())
			pStyleLevel.leftMargin = (LONG)(pptxStyle.marL.get_value_or(0));

		if (pptxStyle.indent.is_init())
			pStyleLevel.indent = (LONG)(pptxStyle.indent.get_value_or(0));

		if (pptxStyle.algn.is_init())
			pStyleLevel.textAlignment = (WORD)(GetTextAlignFromStr(pptxStyle.algn.get_value_or(_T("l"))));

		if (pptxStyle.rtl.is_init())
			pStyleLevel.textDirection = (WORD)((pptxStyle.rtl.get_value_or(false))?1:0);

		if (pptxStyle.fontAlgn.is_init())
			pStyleLevel.fontAlign = (WORD)GetFontAlignFromStr(pptxStyle.fontAlgn->get());

		int defTab = pptxStyle.defTabSz.get_value_or(376300);
		size_t nCountTabs = pptxStyle.tabLst.GetCount();
		for (size_t i = 0; i < nCountTabs; ++i)
		{
			pStyleLevel.tabStops.Add((DWORD)(pptxStyle.tabLst[i].pos.get_value_or(defTab)));
		}

		if (pptxStyle.ParagraphBullet.is_init())
		{
			pStyleLevel.hasBullet = (BOOL)pptxStyle.ParagraphBullet.has_bullet();
		}

		if (pptxStyle.buColor.is_init())
		{
			if (pptxStyle.buColor.is<PPTX::Logic::BuClr>())
			{
				PPTX::Logic::UniColor oBulletColor = pptxStyle.buColor.as<PPTX::Logic::BuClr>().Color;
				pStyleLevel.bulletColor = new CColor();
				pStyleLevel.bulletColor.get() = oBulletColor.GetBGRA();
				if (oBulletColor.is<PPTX::Logic::SchemeClr>())
				{
					pStyleLevel.bulletColor->m_lSchemeIndex = GetColorIndexFromStr(oBulletColor.as<PPTX::Logic::SchemeClr>().val.get());
				}
			}
		}
	}

	void ParagraphPropertiesConvert(const nullable<PPTX::Logic::TextParagraphPr>& pptxStyle, NSPresentationEditor::CTextPFRun& pStyleLevel, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		if (pptxStyle.is_init())
			ParagraphPropertiesConvert(*pptxStyle, pStyleLevel, arFonts);
	}

	void TextStyleLevelConvert(const PPTX::Logic::TextParagraphPr& pptxStyle, NSPresentationEditor::CTextStyleLevel& pStyleLevel, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		ParagraphPropertiesConvert(pptxStyle, pStyleLevel.m_oPFRun, arFonts);

		if (pptxStyle.defRPr.is_init())
			RunPropertiesConvert(pptxStyle.defRPr.get(), pStyleLevel.m_oCFRun, arFonts);
	}

	void TextStyleConvert(const PPTX::Logic::TextListStyle& pptxStyle, NSPresentationEditor::CTextStyles& pStyle, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		for (int i = 0; i < 10; i++)
		{
			if (pptxStyle.levels[i].is_init())
			{
				if (!pStyle.m_pLevels[i].is_init())
				{
					pStyle.m_pLevels[i] = new NSPresentationEditor::CTextStyleLevel();
				}

				TextStyleLevelConvert(pptxStyle.levels[i].get(), pStyle.m_pLevels[i].get(), arFonts);
			}
		}
	}

	void TextStyleConvert(const nullable<PPTX::Logic::TextParagraphPr>* pStyles, NSPresentationEditor::CTextStyles& pStyle, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		for (int i = 0; i < 10; i++)
		{
			if (pStyles[i].is_init())
			{
				if (!pStyle.m_pLevels[i].is_init())
				{
					pStyle.m_pLevels[i] = new NSPresentationEditor::CTextStyleLevel();
				}

				TextStyleLevelConvert(pStyles[i].get(), pStyle.m_pLevels[i].get(), arFonts);
			}
		}
	}

	void TextStyleConvert(const nullable<PPTX::Logic::TextListStyle>& pptxStyle, NSPresentationEditor::CTextStyles& pStyle, CAtlArray<NSPresentationEditor::CFont>& arFonts)
	{
		if (pptxStyle.is_init())
			TextStyleConvert(pptxStyle.get(), pStyle, arFonts);
	}

	void CreateDefaultStyle(NSPresentationEditor::CTextStyles& pStyle, NSPresentationEditor::CTheme* pTheme)
	{
		for (int i = 0; i < 10; ++i)
		{
			if (!pStyle.m_pLevels[i].is_init())
				pStyle.m_pLevels[i] = new NSPresentationEditor::CTextStyleLevel();

			NSPresentationEditor::CTextPFRun* pPF = &pStyle.m_pLevels[i]->m_oPFRun;
			NSPresentationEditor::CTextCFRun* pCF = &pStyle.m_pLevels[i]->m_oCFRun;

			pPF->textAlignment	= (WORD)0;
			pPF->leftMargin		= (LONG)0;
			pPF->indent			= (LONG)0;
			pPF->defaultTabSize	= 376300;
			pPF->fontAlign		= (WORD)0;
			pPF->wrapFlags		= (WORD)0x02;
			pPF->textDirection	= (WORD)0;
			pPF->lineSpacing	= (LONG)100;
			pPF->spaceBefore	= (LONG)0;
			pPF->spaceAfter		= (LONG)0;
			
			pCF->FontBold		= FALSE;
			pCF->FontItalic		= FALSE;
			pCF->FontUnderline	= FALSE;
			pCF->FontStrikeout	= FALSE;

			pCF->Size			= 18;
			pCF->BaseLineOffset = (double)0;
			pCF->Cap			= (WORD)0;

			pCF->FontProperties = new NSPresentationEditor::CFontProperties();
			pCF->FontProperties->SetFont(pTheme->m_arFonts[0]);

			pCF->Color			= new NSPresentationEditor::CColor();
		}
	}
}
	