#include "stdafx.h"
#include "Converter.h"

namespace Odp2DrawingXML
{
	Converter::Converter()
	{
	}

	Converter::~Converter()
	{
	}

	DWORD Converter::Convert(const Odp::Folder& pFolder, CDocument& lDocument)
	{
		lDocument.Clear();
		m_Folder = &pFolder;

		double dDuration = 0.0;
		size_t i;

		lDocument.m_arThemes.Add();
		
		CTheme* pTheme = &lDocument.m_arThemes[0];
		// fonts
		pTheme->m_arFonts.Add();
		pTheme->m_arFonts[0].Name = _T("Arial");
		pTheme->m_arFonts.Add();
		pTheme->m_arFonts[1].Name = _T("Arial");

		// colors
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));
		pTheme->m_arColorScheme.Add(CColor::CreateColor(0));

		pTheme->m_bIsBackground = TRUE;
		pTheme->m_oBackground.Type = c_BrushTypeSolid;
		pTheme->m_oBackground.Color1 = 0xFFFFFFFF;
		pTheme->m_oBackground.Alpha1 = 0xFF;

		// layouts
		pTheme->m_arLayouts.Add();
		pTheme->m_arLayouts[0].m_bIsBackground = false;
		pTheme->m_arLayouts[0].m_bShowMasterShapes = true;
		pTheme->m_arLayouts[0].m_bUseThemeColorScheme = true;

		for(i = 0; i < pFolder.Content->body->Slides->size(); i++)
		{
			SlideConvert(i, &dDuration, lDocument);
			TransitionConvert(i, lDocument);
		}

		if (0 < lDocument.m_arSlides.GetCount())
		{
			lDocument.m_oInfo.m_lMillimetresHor = lDocument.m_arSlides[0].m_lWidth;
			lDocument.m_oInfo.m_lMillimetresVer = lDocument.m_arSlides[0].m_lHeight;
			lDocument.m_oInfo.m_lUnitsHor		= lDocument.m_oInfo.m_lMillimetresHor;
			lDocument.m_oInfo.m_lUnitsVer		= lDocument.m_oInfo.m_lMillimetresVer;
		}

		lDocument.CalculateEditor(lDocument.m_oInfo);
		return 1;
	}
} // namespace Odp2DrawingXML