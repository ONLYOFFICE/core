#include "stdafx.h"
#include "./../Converter.h"

namespace Odp2DrawingXML
{
	void Converter::CustomShapeConvert(const Odp::Content::CustomShape& pCustShape, CSlide& pEContainer, bool master)const
	{
		//CShapeElement* pShape = new CShapeElement(NSBaseShape::odp, (int)OdpShapes::sptCRect);
		CShapeElement* pShape = new CShapeElement(CString(pCustShape.geometry->GetODString().c_str()));

		pShape->m_dStartTime = 0;
		pShape->m_dEndTime = pEContainer.m_dDuration;

		pShape->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
		pShape->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth;
		pShape->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight;
/*
		pShape->m_oShape.m_oBrush.m_Alpha1 = 255;
		pShape->m_oShape.m_oBrush.m_Alpha2 = 255;
		pShape->m_oShape.m_oPen.m_nAlpha = 255;
*/
		RECT rcBounds;
		pCustShape.GetMmRect(rcBounds);
		pShape->m_rcBoundsOriginal.left		= rcBounds.left;
		pShape->m_rcBoundsOriginal.top		= rcBounds.top;
		pShape->m_rcBoundsOriginal.right	= rcBounds.right;
		pShape->m_rcBoundsOriginal.bottom	= rcBounds.bottom;
		pShape->m_rcBounds = pShape->m_rcBoundsOriginal;

		Odt::Logic::Properties TextProps;
		Odt::Logic::Properties GraphicProps;
		if(pCustShape.text_style_name.is_init())
		{
			std::string style = pCustShape.text_style_name.get();
			TextProps = m_Folder->GetPropertiesFromStyle(style, master);
		}
		if(pCustShape.style_name.is_init())
		{
			std::string style = pCustShape.style_name.get();
			GraphicProps = m_Folder->GetPropertiesFromStyle(style, master);
		}
		Odt::Logic::Properties properties;
		properties = Odt::Logic::Properties::merge(GraphicProps, TextProps);
		FillPenBrush(properties, *pShape);

		TextVectorConvert(pCustShape.text.get(), pShape->m_oShape.m_oText, properties, master);
/*
		double AutoFit = pShape.txBody->bodyPr->Fit.fontScale.get_value_or(100000)/100000.0;
		TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, AutoFit);
*/
		pShape->SetupProperties(&pEContainer, NULL, NULL);
		pEContainer.m_arElements.Add(pShape);
	}
}