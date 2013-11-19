#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2OfficeDrawing
{
	void Converter::SlideConvert(const boost::shared_ptr<PPTX::Slide> pSource, NSPresentationEditor::CSlide& pDest)
	{
		pDest.m_bUseLayoutColorScheme = true;
		if(pSource->clrMapOvr.is_init())
		{
			if(pSource->clrMapOvr->overrideClrMapping.is_init())
			{
				pDest.m_bUseLayoutColorScheme = false;

				CColor lColor;
				lColor.m_lSchemeIndex = 0;
				pDest.m_arColorScheme.Add(lColor);//phClr
				lColor.m_lSchemeIndex = -1;
				lColor = pSource->GetBGRAFromMap("bg1");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("tx1");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("bg2");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("tx2");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("accent1");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("accent2");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("accent3");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("accent4");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("accent5");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("accent6");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("hlink");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->GetBGRAFromMap("folHlink");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->Master->GetBGRAFromMap("lt1");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->Master->GetBGRAFromMap("dk1");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->Master->GetBGRAFromMap("lt2");
				pDest.m_arColorScheme.Add(lColor);
				lColor = pSource->Master->GetBGRAFromMap("dk2");
				pDest.m_arColorScheme.Add(lColor);
			}
		}

		BackgroundConvert(pSource->cSld->bg, pDest.m_arElements);
		size_t start = 0;
		if(pSource->cSld->bg.is_init())
		{
			start = 1;
			pDest.m_arElements.GetAt(0)->m_bIsChangeable = false;

			pDest.m_arElements.GetAt(0)->m_rcBoundsOriginal.top = 0;
			pDest.m_arElements.GetAt(0)->m_rcBoundsOriginal.left = 0;
			pDest.m_arElements.GetAt(0)->m_rcBoundsOriginal.right = pDest.m_lOriginalWidth;
			pDest.m_arElements.GetAt(0)->m_rcBoundsOriginal.bottom = pDest.m_lOriginalHeight;

			pDest.m_arElements.GetAt(0)->m_oMetric.m_lUnitsHor = pDest.m_lOriginalWidth;
			pDest.m_arElements.GetAt(0)->m_oMetric.m_lUnitsVer = pDest.m_lOriginalHeight;
			pDest.m_arElements.GetAt(0)->m_oMetric.m_lMillimetresHor = pDest.m_lOriginalWidth/36000;
			pDest.m_arElements.GetAt(0)->m_oMetric.m_lMillimetresVer = pDest.m_lOriginalHeight/36000;
		}
		GroupConvert(pSource->cSld->spTree, pDest.m_arElements);
		for(size_t i = start; i < pDest.m_arElements.GetCount(); i++)
		{
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lUnitsHor = pDest.m_lOriginalWidth;
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lUnitsVer = pDest.m_lOriginalHeight;
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lMillimetresHor = pDest.m_lOriginalWidth/36000;
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lMillimetresVer = pDest.m_lOriginalHeight/36000;

			pDest.m_arElements.GetAt(i)->m_dStartTime = 0;
			pDest.m_arElements.GetAt(i)->m_dEndTime = pDest.m_dDuration;

			pDest.m_arElements.GetAt(i)->m_bIsChangeable = true;		// можно ли редактировать элемент
			pDest.m_arElements.GetAt(i)->m_bIsLayoutElement = false;	// был ли элемент вставлен из шаблона, чтобы можно было менять шаблон
		}
/*
		LONG m_lThemeID;
		LONG m_lLayoutID;

		CSlideShowInfo m_oSlideShow;

		double m_dStartTime;
		double m_dEndTime;
		double m_dDuration;
*/
	}
}
