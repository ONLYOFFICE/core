#include "stdafx.h"
#include "./../Converter.h"
#include "Utility.h"

namespace Odp2DrawingXML
{
	void Converter::SlideConvert(const DWORD SldNum, double* Duration, CDrawingDocument& pDocument)const
	{
		std::string master = m_Folder->Content->body->Slides->at(SldNum).MasterPageName.get();
		const Odp::Styles::MasterPage& MasterPage = m_Folder->GetMasterPage(master);
		std::string layout = MasterPage.Layout.get();
		const Odp::Styles::PageLayout& PageLayout = m_Folder->GetPageLayout(layout);

		CElementsContainer* lEContainer = new CElementsContainer;
		lEContainer->m_lOriginalWidth = (long)(PageLayout.Properties->page_width->value(UniversalUnit::Mm));
		lEContainer->m_lOriginalHeight = (long)(PageLayout.Properties->page_height->value(UniversalUnit::Mm));
		lEContainer->m_lWidth = lEContainer->m_lOriginalWidth;
		lEContainer->m_lHeight = lEContainer->m_lOriginalHeight;

		lEContainer->m_dStartTime = *Duration;
		lEContainer->m_dDuration = 30000.0;
		*Duration += 30000.0;
		lEContainer->m_dEndTime = *Duration;

		AddBackground(master, *lEContainer);

		for(size_t i = 0; i < m_Folder->Content->body->Slides->at(SldNum).Elements->size(); i++)
		{
			DrawingElementConvert(m_Folder->Content->body->Slides->at(SldNum).Elements->at(i), *lEContainer, false);
		}

		lEContainer->NormalizeCoordinates();
		pDocument.m_arPages.Add(lEContainer);
	}

	void Converter::AddBackground(const std::string& MasterName, CElementsContainer& pEContainer)const
	{
		const Odp::Styles::MasterPage& MasterPage = m_Folder->GetMasterPage(MasterName);

		Odt::Logic::Properties properties = m_Folder->GetPropertiesFromStyle(MasterPage.StyleName.get(), true);
		if((properties.DrawingPage.is_init()) && (properties.DrawingPage->Fill.get_value_or("none") != "none"))
		{
			CShapeElement* lpSlideBackground = new CShapeElement(NSBaseShape::odp, (int)OdpShapes::sptCRect);

			lpSlideBackground->m_dStartTime = 0;
			lpSlideBackground->m_dEndTime = pEContainer.m_dDuration;

			lpSlideBackground->m_rcBoundsOriginal.top = 0;
			lpSlideBackground->m_rcBoundsOriginal.left = 0;
			lpSlideBackground->m_rcBoundsOriginal.right = pEContainer.m_lWidth;
			lpSlideBackground->m_rcBoundsOriginal.bottom = pEContainer.m_lHeight;

			lpSlideBackground->m_oMetric.m_lUnitsHor = pEContainer.m_lWidth;
			lpSlideBackground->m_oMetric.m_lUnitsVer = pEContainer.m_lHeight;
			lpSlideBackground->m_oMetric.m_lMillimetresHor = pEContainer.m_lWidth;
			lpSlideBackground->m_oMetric.m_lMillimetresVer = pEContainer.m_lHeight;

			lpSlideBackground->m_oShape.m_oPen.m_nAlpha = 0;

			FillBackground(properties, *lpSlideBackground);

			lpSlideBackground->m_oShape.SetProperties(NULL, &pEContainer);
			pEContainer.m_arElements.Add(lpSlideBackground);
		}

		for(size_t i = 0; i < MasterPage.Elements->size(); i++)
		{
			if(MasterPage.Elements->at(i).is<Odp::Content::Frame>())
			{
				if(!MasterPage.Elements->at(i).as<Odp::Content::Frame>().Class.is_init())
					FrameConvert(MasterPage.Elements->at(i).as<Odp::Content::Frame>(), pEContainer, true);
			}
			else if(MasterPage.Elements->at(i).is<Odp::Content::CustomShape>())
			{
				CustomShapeConvert(MasterPage.Elements->at(i).as<Odp::Content::CustomShape>(), pEContainer, true);
			}
			else if(MasterPage.Elements->at(i).is<Odp::Content::Group>())
			{
				GroupConvert(MasterPage.Elements->at(i).as<Odp::Content::Group>(), pEContainer, true);
			}
			else if(MasterPage.Elements->at(i).is<Odp::Content::Line>())
			{
				LineConvert(MasterPage.Elements->at(i).as<Odp::Content::Line>(), pEContainer, true);
			}
			else if(MasterPage.Elements->at(i).is<Odp::Content::Rect>())
			{
				RectConvert(MasterPage.Elements->at(i).as<Odp::Content::Rect>(), pEContainer, true);
			}
		}
	}

	void Converter::DrawingElementConvert(const Odp::Content::DrawingElement& pDrawElem, CElementsContainer& pEContainer, bool master)const
	{
		if(pDrawElem.is<Odp::Content::Frame>())
			FrameConvert(pDrawElem.as<Odp::Content::Frame>(), pEContainer, master);
		else if(pDrawElem.is<Odp::Content::CustomShape>())
		{
			CustomShapeConvert(pDrawElem.as<Odp::Content::CustomShape>(), pEContainer, master);
		}
		else if(pDrawElem.is<Odp::Content::Group>())
		{
			GroupConvert(pDrawElem.as<Odp::Content::Group>(), pEContainer, master);
		}
		else if(pDrawElem.is<Odp::Content::Line>())
		{
			LineConvert(pDrawElem.as<Odp::Content::Line>(), pEContainer, master);
		}
		else if(pDrawElem.is<Odp::Content::Rect>())
		{
			RectConvert(pDrawElem.as<Odp::Content::Rect>(), pEContainer, master);
		}
	}

	void Converter::GroupConvert(const Odp::Content::Group& pGroup, CElementsContainer& pEContainer, bool master)const
	{
		for(size_t i = 0; i < pGroup.Elements->size(); i++)
		{
			DrawingElementConvert(pGroup.Elements->at(i), pEContainer, master);
		}
	}
}