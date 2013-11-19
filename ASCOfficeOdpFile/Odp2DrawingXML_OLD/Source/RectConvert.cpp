#include "stdafx.h"
#include "./../Converter.h"

namespace Odp2DrawingXML
{
	void Converter::RectConvert(const Odp::Content::Rect& pRect, CElementsContainer& pEContainer, bool master)const
	{
		CShapeElement* pShape = new CShapeElement(NSBaseShape::odp, (int)OdpShapes::sptCRect);
		pShape->m_dStartTime = 0;
		pShape->m_dEndTime = pEContainer.m_dDuration;

		pShape->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		pShape->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight;

		Odt::Logic::Properties TextProps;
		Odt::Logic::Properties GraphicProps;
		Odt::Logic::Properties PresProps;
		if(pRect.TextStyleName.is_init())
		{
			std::string style = pRect.TextStyleName.get();
			TextProps = m_Folder->GetPropertiesFromStyle(style, master);
		}
		if(pRect.DrawStyleName.is_init())
		{
			std::string style = pRect.DrawStyleName.get();
			GraphicProps = m_Folder->GetPropertiesFromStyle(style, master);
		}
		if(pRect.PresentationStyleName.is_init())
		{
			std::string style = pRect.PresentationStyleName.get();
			PresProps = m_Folder->GetPropertiesFromStyle(style, master);
		}

		Odt::Logic::Properties properties;
		properties = Odt::Logic::Properties::merge(GraphicProps, TextProps);
		properties = Odt::Logic::Properties::merge(PresProps, properties);
/*
		std::string style = pRect.StyleName.get();
		Odt::Logic::Properties GraphicProps;
		GraphicProps = m_Folder->GetPropertiesFromStyle(style, master);

		Odt::Logic::Properties TextProps;
		if(pRect.TextStyleName.is_init())
		{
			style = pRect.TextStyleName.get();
			TextProps = m_Folder->GetPropertiesFromStyle(style, master);
		}

		Odt::Logic::Properties properties;
		properties = Odt::Logic::Properties::merge(GraphicProps, TextProps);
*/
		FillPenBrush(properties, *pShape);

		pRect.GetMmRect(pShape->m_rcBoundsOriginal);

		TextVectorConvert(pRect.Text.get(), pShape->m_oShape.m_oText, properties, master);
/*
			double AutoFit = pShape.txBody->bodyPr->Fit.fontScale.get_value_or(100000)/100000.0;
			TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, AutoFit);
*/
		pShape->m_oShape.SetProperties(NULL, &pEContainer);
		pEContainer.m_arElements.Add(pShape);
	}
}