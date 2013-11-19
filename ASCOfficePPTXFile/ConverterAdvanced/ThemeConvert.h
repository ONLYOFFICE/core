#pragma once

#include "../stdafx.h"
#include "TextStyleConvert.h"
#include "ShapeConvert.h"

namespace PPTX2EditorAdvanced
{
	LONG ThemeConvert(smart_ptr<PPTX::SlideMaster>& Master, CPresentationInfo& oPresentation)
	{
		if (!Master.IsInit())
			return -1;
		
		LONG lIndex = oPresentation.GetIndexTheme(Master);

		if (-1 != lIndex)
		{
			// тема уже сконвертирована - возвращаем ее индекс
			return lIndex;
		}

		CPresentationInfo::CThemeInfo* pThemeInfo = oPresentation.AddTheme(Master);

		NSPresentationEditor::CTheme* pTheme = &pThemeInfo->m_oTheme;

		// background
		if(Master->cSld.bg.is_init())
		{
			pTheme->m_bIsBackground = true;

			PPTX::Logic::BgPr bg;
			DWORD ARGB = 0;
			Master->cSld.bg->GetBackground(bg, ARGB);
			DWORD BGRA = (ARGB & 0xFF) << 24;
			BGRA += (ARGB & 0xFF00) << 8;
			BGRA += (ARGB & 0xFF0000) >> 8;
			BGRA += (ARGB & 0xFF000000) >> 24;
			FillToBrush(bg.Fill, BGRA, pTheme->m_oBackground);
		}

		// brushes
		if (FALSE)
		{
			size_t nCount = Master->Theme->themeElements.fmtScheme.fillStyleLst.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				NSPresentationEditor::CBrush lBrush;
				FillToBrush(Master->Theme->themeElements.fmtScheme.fillStyleLst[i], 0, lBrush);
				pTheme->m_arBrushes.Add(lBrush);
			}
		}

		// pen
		if (FALSE)
		{
			size_t nCount = Master->Theme->themeElements.fmtScheme.lnStyleLst.GetCount();
			for (size_t i = 0; i < nCount; ++i)
			{
				NSPresentationEditor::CPen lPen;
				LnToPen(Master->Theme->themeElements.fmtScheme.lnStyleLst[i], 0, lPen);
				pTheme->m_arPens.Add(lPen);
			}
		}

		// colorscheme
		NSPresentationEditor::CColor lColor;
		lColor.m_lSchemeIndex = 0;
		pTheme->m_arColorScheme.Add(lColor);//phClr
		lColor.m_lSchemeIndex = -1;

		lColor = Master->GetBGRAFromMap(_T("bg1"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("tx1"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("bg2"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("tx2"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("accent1"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("accent2"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("accent3"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("accent4"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("accent5"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("accent6"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("hlink"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("folHlink"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("lt1"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("dk1"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("lt2"));
		pTheme->m_arColorScheme.Add(lColor);
		lColor = Master->GetBGRAFromMap(_T("dk2"));
		pTheme->m_arColorScheme.Add(lColor);

		// fonts
		NSPresentationEditor::CFont lFont;
		lFont.Name = Master->Theme->themeElements.fontScheme.majorFont.latin.typeface;
		pTheme->m_arFonts.Add(lFont);
		lFont.Name = Master->Theme->themeElements.fontScheme.minorFont.latin.typeface;
		pTheme->m_arFonts.Add(lFont);
		lFont.Name = Master->Theme->themeElements.fontScheme.majorFont.ea.typeface;
		pTheme->m_arFonts.Add(lFont);
		lFont.Name = Master->Theme->themeElements.fontScheme.minorFont.ea.typeface;
		pTheme->m_arFonts.Add(lFont);
		lFont.Name = Master->Theme->themeElements.fontScheme.majorFont.cs.typeface;
		pTheme->m_arFonts.Add(lFont);
		lFont.Name = Master->Theme->themeElements.fontScheme.minorFont.cs.typeface;
		pTheme->m_arFonts.Add(lFont);

		// textstyles
		NSPresentationEditor::CTextStyles lPPTXDefault;
		CreateDefaultStyle(lPPTXDefault, pTheme);

		NSPresentationEditor::CTextStyles lNoPlaceHoldersStyles;
		
		NSPresentationEditor::CTextStyles lDefaultStyles;
		if (Master->Theme.is_init() && Master->Theme->Presentation.is_init())
		{
			TextStyleConvert(Master->Theme->Presentation->defaultTextStyle, lDefaultStyles, pTheme->m_arFonts);
		}

		// default
		if (Master->txStyles.is_init())
		{
			TextStyleConvert(Master->txStyles->otherStyle, lNoPlaceHoldersStyles, pTheme->m_arFonts);
			lNoPlaceHoldersStyles.ApplyAfter(lDefaultStyles);
		}
		pTheme->m_pStyles[1] = lDefaultStyles;
		pTheme->m_pStyles[2] = lDefaultStyles;
		pTheme->m_pStyles[3] = lDefaultStyles;

		if (Master->txStyles.is_init())
		{
			NSPresentationEditor::CTextStyles lTitleStyle;
			NSPresentationEditor::CTextStyles lBodyStyle;
			NSPresentationEditor::CTextStyles lOtherStyle;

			TextStyleConvert(Master->txStyles->titleStyle,	lTitleStyle, pTheme->m_arFonts);
			TextStyleConvert(Master->txStyles->bodyStyle,	lBodyStyle, pTheme->m_arFonts);
			TextStyleConvert(Master->txStyles->otherStyle,	lOtherStyle, pTheme->m_arFonts);

			pTheme->m_pStyles[1].ApplyAfter(lTitleStyle);
			pTheme->m_pStyles[2].ApplyAfter(lBodyStyle);
			pTheme->m_pStyles[3].ApplyAfter(lOtherStyle);
		}		

		pTheme->m_pStyles[0] = lNoPlaceHoldersStyles;

		pTheme->m_pStyles[0].ApplyBefore(lPPTXDefault);
		pTheme->m_pStyles[1].ApplyBefore(lPPTXDefault);
		pTheme->m_pStyles[2].ApplyBefore(lPPTXDefault);
		pTheme->m_pStyles[3].ApplyBefore(lPPTXDefault);

		// elements
		AddNonPlaceholders(Master->cSld.spTree, pTheme->m_arElements, oPresentation, pTheme);

		return (LONG)(oPresentation.m_arThemes.GetCount() - 1);
	}
}