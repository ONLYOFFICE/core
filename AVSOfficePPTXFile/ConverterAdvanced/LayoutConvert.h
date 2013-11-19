#pragma once

#include "../stdafx.h"
#include "ShapeConvert.h"

namespace PPTX2EditorAdvanced
{
	LONG LayoutConvert(smart_ptr<PPTX::SlideLayout>& Layout, CPresentationInfo& oPresentation)
	{
		if (!Layout.IsInit())
			return -1;

		LONG lIndexTheme = oPresentation.GetIndexTheme(Layout->Master);

		if (-1 == lIndexTheme)
		{
			// такого быть не может
			return -1;
		}

		CPresentationInfo::CThemeInfo* pThemeInfo = oPresentation.m_arThemes[lIndexTheme];

		LONG lIndexLayout = pThemeInfo->GetIndexLayout(Layout);

		if (-1 != lIndexLayout)
		{
			// он уже сконвертирован
			return lIndexLayout;
		}

		NSPresentationEditor::CLayout* pLayout = pThemeInfo->AddLayout(Layout);

		if (Layout->attrType.is_init())
			pLayout->m_strLayoutType = Layout->attrType->get();
		else
			pLayout->m_strLayoutType = _T("title");

		pLayout->m_bShowMasterShapes = true;
		if (Layout->showMasterSp.is_init())
			pLayout->m_bShowMasterShapes = Layout->showMasterSp.get();

		// background
		if(Layout->cSld.bg.is_init())
		{
			pLayout->m_bIsBackground = true;

			PPTX::Logic::BgPr bg;
			DWORD ARGB = 0;
			Layout->cSld.bg->GetBackground(bg, ARGB);
			DWORD BGRA = (ARGB & 0xFF) << 24;
			BGRA += (ARGB & 0xFF00) << 8;
			BGRA += (ARGB & 0xFF0000) >> 8;
			BGRA += (ARGB & 0xFF000000) >> 24;
			FillToBrush(bg.Fill, BGRA, pLayout->m_oBackground);
		}

		// colorscheme
		if (Layout->clrMapOvr.is_init())
		{
			if (Layout->clrMapOvr->overrideClrMapping.is_init())
			{
				pLayout->m_bUseThemeColorScheme = false;

				NSPresentationEditor::CColor lColor;
				lColor.m_lSchemeIndex = 0;
				pLayout->m_arColorScheme.Add(lColor);//phClr
				lColor.m_lSchemeIndex = -1;

				lColor = Layout->GetBGRAFromMap(_T("bg1"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("tx1"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("bg2"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("tx2"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("accent1"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("accent2"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("accent3"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("accent4"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("accent5"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("accent6"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("hlink"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("folHlink"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("lt1"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("dk1"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("lt2"));
				pLayout->m_arColorScheme.Add(lColor);
				lColor = Layout->GetBGRAFromMap(_T("dk2"));
				pLayout->m_arColorScheme.Add(lColor);
			}
		}

		// elements
		GroupConvert(Layout->cSld.spTree, pLayout->m_arElements, oPresentation, &pThemeInfo->m_oTheme);

		return (LONG)(pThemeInfo->m_oTheme.m_arLayouts.GetCount() - 1);
	}
}