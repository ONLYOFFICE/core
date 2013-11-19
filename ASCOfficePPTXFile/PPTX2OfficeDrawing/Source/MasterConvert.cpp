#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2OfficeDrawing
{
	void Converter::MasterConvert(const boost::shared_ptr<PPTX::SlideMaster> pMaster, NSPresentationEditor::CDocument& pDocument, int cx, int cy)
	{
		CTheme lTheme;
		for(std::list<PPTX::Logic::XmlId>::const_iterator iter = pMaster->sldLayoutIdLst->begin();
			iter != pMaster->sldLayoutIdLst->end(); iter++)
		{
			CLayout lLayout;

			lLayout.m_lOriginalWidth = cx;
			lLayout.m_lOriginalHeight = cy;

			BackgroundConvert(pMaster->cSld->bg, lLayout.m_arElements);
			if(pMaster->cSld->bg.is_init())
			{
				lLayout.m_arElements.GetAt(0)->m_bIsChangeable = false;

				lLayout.m_arElements.GetAt(0)->m_rcBoundsOriginal.top = 0;
				lLayout.m_arElements.GetAt(0)->m_rcBoundsOriginal.left = 0;
				lLayout.m_arElements.GetAt(0)->m_rcBoundsOriginal.right = cx;
				lLayout.m_arElements.GetAt(0)->m_rcBoundsOriginal.bottom = cy;

				lLayout.m_arElements.GetAt(0)->m_oMetric.m_lUnitsHor = cx;
				lLayout.m_arElements.GetAt(0)->m_oMetric.m_lUnitsVer = cy;
				lLayout.m_arElements.GetAt(0)->m_oMetric.m_lMillimetresHor = cx/36000;
				lLayout.m_arElements.GetAt(0)->m_oMetric.m_lMillimetresVer = cy/36000;
			}

			if(boost::shared_dynamic_cast<PPTX::SlideLayout, OOX::File>((*pMaster)[iter->rid.get()])->showMasterSp.get_value_or(true))
				AddNonPlaceholders(pMaster->cSld->spTree, lLayout.m_arElements);
			LayoutConvert(boost::shared_dynamic_cast<PPTX::SlideLayout, OOX::File>((*pMaster)[iter->rid.get()]), lLayout);
			lTheme.m_arLayouts.Add(lLayout);
		}

		for(std::list<PPTX::Logic::UniFill>::const_iterator iter = pMaster->Theme->themeElements->fmtScheme->fillStyleLst->begin();
			iter != pMaster->Theme->themeElements->fmtScheme->fillStyleLst->end(); iter++)
		{
			CBrush lBrush;
			FillToBrush(*iter, lBrush);
			lTheme.m_arBrushes.Add(lBrush);
		}

		for(std::list<PPTX::Logic::Ln>::const_iterator iter = pMaster->Theme->themeElements->fmtScheme->lnStyleLst->begin();
			iter != pMaster->Theme->themeElements->fmtScheme->lnStyleLst->end(); iter++)
		{
			CPen lPen;
			LineToPen(*iter, lPen);
			lTheme.m_arPens.Add(lPen);
		}

		CColor lColor;
		lColor.m_lSchemeIndex = 0;
		lTheme.m_arColorScheme.Add(lColor);//phClr
		lColor.m_lSchemeIndex = -1;
		lColor = pMaster->GetBGRAFromMap("bg1");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("tx1");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("bg2");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("tx2");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("accent1");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("accent2");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("accent3");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("accent4");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("accent5");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("accent6");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("hlink");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("folHlink");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("lt1");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("dk1");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("lt2");
		lTheme.m_arColorScheme.Add(lColor);
		lColor = pMaster->GetBGRAFromMap("dk2");
		lTheme.m_arColorScheme.Add(lColor);

		CFont lFont;
		lFont.SetName(CString(pMaster->Theme->themeElements->fontScheme->majorFont->latin->typeface.get().c_str()));
		lTheme.m_arFonts.Add(lFont);
		lFont.SetName(CString(pMaster->Theme->themeElements->fontScheme->minorFont->latin->typeface.get().c_str()));
		lTheme.m_arFonts.Add(lFont);
		lFont.SetName(CString(pMaster->Theme->themeElements->fontScheme->majorFont->ea->typeface.get().c_str()));
		lTheme.m_arFonts.Add(lFont);
		lFont.SetName(CString(pMaster->Theme->themeElements->fontScheme->minorFont->ea->typeface.get().c_str()));
		lTheme.m_arFonts.Add(lFont);
		lFont.SetName(CString(pMaster->Theme->themeElements->fontScheme->majorFont->cs->typeface.get().c_str()));
		lTheme.m_arFonts.Add(lFont);
		lFont.SetName(CString(pMaster->Theme->themeElements->fontScheme->minorFont->cs->typeface.get().c_str()));
		lTheme.m_arFonts.Add(lFont);

		CTextStyle lTextStyle;
		TextStyleConvert(pMaster->Theme->Presentation->defaultTextStyle, lTextStyle);
		lTheme.m_arTextStyles.Add(lTextStyle);
		if(pMaster->txStyles.is_init())
		{
			CTextStyle lTitleStyle;
			CTextStyle lBodyStyle;
			CTextStyle lOtherStyle;

			TextStyleConvert(pMaster->txStyles->titleStyle, lTitleStyle);
			TextStyleConvert(pMaster->txStyles->bodyStyle,	lBodyStyle);
			TextStyleConvert(pMaster->txStyles->otherStyle, lOtherStyle);
		}

		//CAtlArray<CEffects>		m_arEffects;

		pDocument.AddTheme(lTheme);
	}
}