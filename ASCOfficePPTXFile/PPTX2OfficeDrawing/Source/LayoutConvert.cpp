#include "stdafx.h"
#include "./../Converter.h"

namespace PPTX2OfficeDrawing
{
	void Converter::LayoutConvert(const boost::shared_ptr<PPTX::SlideLayout> pSource, NSPresentationEditor::CLayout& pDest)
	{
		pDest.m_bUseThemeColorScheme = true;
		if(pSource->clrMapOvr.is_init())
		{
			if(pSource->clrMapOvr->overrideClrMapping.is_init())
			{
				pDest.m_bUseThemeColorScheme = false;

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
		if(pSource->cSld->bg.is_init())
		{
			size_t start = pDest.m_arElements.GetCount() - 1;
			pDest.m_arElements.GetAt(start)->m_rcBoundsOriginal.top = 0;
			pDest.m_arElements.GetAt(start)->m_rcBoundsOriginal.left = 0;
			pDest.m_arElements.GetAt(start)->m_rcBoundsOriginal.right = pDest.m_lOriginalWidth;
			pDest.m_arElements.GetAt(start)->m_rcBoundsOriginal.bottom = pDest.m_lOriginalHeight;

			pDest.m_arElements.GetAt(start)->m_oMetric.m_lUnitsHor = pDest.m_lOriginalWidth;
			pDest.m_arElements.GetAt(start)->m_oMetric.m_lUnitsVer = pDest.m_lOriginalHeight;
			pDest.m_arElements.GetAt(start)->m_oMetric.m_lMillimetresHor = pDest.m_lOriginalWidth/36000;
			pDest.m_arElements.GetAt(start)->m_oMetric.m_lMillimetresVer = pDest.m_lOriginalHeight/36000;
		}
		GroupConvert(pSource->cSld->spTree, pDest.m_arElements, true);
		for(size_t i = 0; i < pDest.m_arElements.GetCount(); i++)
		{
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lUnitsHor = pDest.m_lOriginalWidth;
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lUnitsVer = pDest.m_lOriginalHeight;
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lMillimetresHor = pDest.m_lOriginalWidth/36000;
			pDest.m_arElements.GetAt(i)->m_oMetric.m_lMillimetresVer = pDest.m_lOriginalHeight/36000;
/*
			lpShapeElement->m_dStartTime = 0;
			lpShapeElement->m_dEndTime = pEContainer.m_dDuration;
*/
			pDest.m_arElements.GetAt(i)->m_bIsChangeable = false;		// можно ли редактировать элемент
			pDest.m_arElements.GetAt(i)->m_bIsLayoutElement = true;		// был ли элемент вставлен из шаблона, чтобы можно было менять шаблон
		}
	}
}
