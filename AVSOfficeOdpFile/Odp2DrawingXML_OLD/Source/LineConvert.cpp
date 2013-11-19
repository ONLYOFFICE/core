#include "stdafx.h"
#include "./../Converter.h"

namespace Odp2DrawingXML
{
	void Converter::LineConvert(const Odp::Content::Line& pLine, CElementsContainer& pEContainer, bool master)const
	{
		CShapeElement* pShape = new CShapeElement(NSBaseShape::odp, (int)OdpShapes::sptCLine);
		pShape->m_dStartTime = 0;
		pShape->m_dEndTime = pEContainer.m_dDuration;

		pShape->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		pShape->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight;

		std::string style = pLine.StyleName.get();
		Odt::Logic::Properties GraphicProps;
		GraphicProps = m_Folder->GetPropertiesFromStyle(style, master);

		Odt::Logic::Properties TextProps;
		if(pLine.TextStyleName.is_init())
		{
			style = pLine.TextStyleName.get();
			TextProps = m_Folder->GetPropertiesFromStyle(style, master);
		}

		Odt::Logic::Properties properties;
		properties = Odt::Logic::Properties::merge(GraphicProps, TextProps);
		FillPenBrush(properties, *pShape);

		pLine.GetMmRect(pShape->m_rcBoundsOriginal);
		pShape->m_oShape.m_pShape->SetAdjustment(4, pEContainer.m_lOriginalWidth);
		pShape->m_oShape.m_pShape->SetAdjustment(5, pEContainer.m_lOriginalHeight);

		pShape->m_oShape.m_pShape->SetAdjustment(0, pShape->m_rcBoundsOriginal.left);
		pShape->m_oShape.m_pShape->SetAdjustment(1, pShape->m_rcBoundsOriginal.top);
		pShape->m_oShape.m_pShape->SetAdjustment(2, pShape->m_rcBoundsOriginal.right - pShape->m_rcBoundsOriginal.left);
		pShape->m_oShape.m_pShape->SetAdjustment(3, pShape->m_rcBoundsOriginal.bottom - pShape->m_rcBoundsOriginal.top);
		pShape->m_rcBoundsOriginal.left = 0;
		pShape->m_rcBoundsOriginal.top = 0;
		pShape->m_rcBoundsOriginal.right = pEContainer.m_lOriginalWidth;
		pShape->m_rcBoundsOriginal.bottom = pEContainer.m_lOriginalHeight;

		TextVectorConvert(pLine.Text.get(), pShape->m_oShape.m_oText, properties, master);
/*
			double AutoFit = pShape.txBody->bodyPr->Fit.fontScale.get_value_or(100000)/100000.0;
			TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, AutoFit);
*/
		pShape->m_oShape.SetProperties(NULL, &pEContainer);
		pEContainer.m_arElements.Add(pShape);
	}
}