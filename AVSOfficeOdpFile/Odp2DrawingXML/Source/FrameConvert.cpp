#include "stdafx.h"
#include "./../Converter.h"

namespace Odp2DrawingXML
{
	void Converter::FrameConvert(const Odp::Content::Frame& pFrame, CSlide& pEContainer, bool master)const
	{
		if((pFrame.image.is_init()) && (!pFrame.object.is_init()) && (!pFrame.Table.is_init()))
		{
			CImageElement* lpImageElement = new CImageElement();

			lpImageElement->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
			lpImageElement->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
			lpImageElement->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth;
			lpImageElement->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight;

			lpImageElement->m_dStartTime = 0;
			lpImageElement->m_dEndTime = pEContainer.m_dDuration;

			RECT rcBounds;
			pFrame.GetMmRect(rcBounds);
			lpImageElement->m_rcBoundsOriginal.left		= rcBounds.left;
			lpImageElement->m_rcBoundsOriginal.top		= rcBounds.top;
			lpImageElement->m_rcBoundsOriginal.right	= rcBounds.right;
			lpImageElement->m_rcBoundsOriginal.bottom	= rcBounds.bottom;
			
			lpImageElement->m_rcBounds = lpImageElement->m_rcBoundsOriginal;
			
			lpImageElement->m_strFileName = CString(m_Folder->GetPath().c_str()) + CString(pFrame.image->href.get().c_str());

			//lpImageElement->SetUpProperties(NULL, &pEContainer);
			pEContainer.m_arElements.Add(lpImageElement);
			return;
		}
		else if(pFrame.text_box.is_init())
		{
			CShapeElement* pTextBox = new CShapeElement(NSBaseShape::odp, (int)OdpShapes::sptCRect);
			pTextBox->m_dStartTime = 0;
			pTextBox->m_dEndTime = pEContainer.m_dDuration;

			pTextBox->m_oMetric.m_lUnitsHor = pEContainer.m_lOriginalWidth;
			pTextBox->m_oMetric.m_lUnitsVer = pEContainer.m_lOriginalHeight;
			pTextBox->m_oMetric.m_lMillimetresHor = pEContainer.m_lOriginalWidth;
			pTextBox->m_oMetric.m_lMillimetresVer = pEContainer.m_lOriginalHeight;

			pTextBox->m_oShape.m_oBrush.Alpha1 = 0;
			pTextBox->m_oShape.m_oBrush.Alpha2 = 0;
			pTextBox->m_oShape.m_oPen.Alpha = 0;

			RECT rcBounds;
			pFrame.GetMmRect(rcBounds);
			pTextBox->m_rcBoundsOriginal.left	= rcBounds.left;
			pTextBox->m_rcBoundsOriginal.top	= rcBounds.top;
			pTextBox->m_rcBoundsOriginal.right	= rcBounds.right;
			pTextBox->m_rcBoundsOriginal.bottom	= rcBounds.bottom;

			pTextBox->m_rcBounds = pTextBox->m_rcBoundsOriginal;

			Odt::Logic::Properties TextProps;
			Odt::Logic::Properties GraphicProps;
			Odt::Logic::Properties PresProps;
			if(pFrame.text_style_name.is_init())
			{
				std::string style = pFrame.text_style_name.get();
				TextProps = m_Folder->GetPropertiesFromStyle(style, master);
			}
			if(pFrame.draw_style_name.is_init())
			{
				std::string style = pFrame.draw_style_name.get();
				GraphicProps = m_Folder->GetPropertiesFromStyle(style, master);
			}
			if(pFrame.presentation_style_name.is_init())
			{
				std::string style = pFrame.presentation_style_name.get();
				PresProps = m_Folder->GetPropertiesFromStyle(style, master);
			}

			Odt::Logic::Properties properties;
			properties = Odt::Logic::Properties::merge(GraphicProps, TextProps);
			properties = Odt::Logic::Properties::merge(PresProps, properties);
			TextVectorConvert(pFrame.text_box->Items.get(), pTextBox->m_oShape.m_oText, properties, master);
/*
			double AutoFit = pShape.txBody->bodyPr->Fit.fontScale.get_value_or(100000)/100000.0;
			TextParse(pShape.txBody.get(), lpShapeElement->m_oShape.m_oText, ShapeProps, AutoFit);
*/
			pEContainer.m_arElements.Add(pTextBox);
		}
		else if(pFrame.Table.is_init())
		{
			RECT rect;
			pFrame.GetMmRect(rect);

			Odt::Logic::Properties TextProps;
			Odt::Logic::Properties GraphicProps;
			Odt::Logic::Properties PresProps;
			if(pFrame.text_style_name.is_init())
			{
				std::string style = pFrame.text_style_name.get();
				TextProps = m_Folder->GetPropertiesFromStyle(style, master);
			}
			if(pFrame.draw_style_name.is_init())
			{
				std::string style = pFrame.draw_style_name.get();
				GraphicProps = m_Folder->GetPropertiesFromStyle(style, master);
			}
			if(pFrame.presentation_style_name.is_init())
			{
				std::string style = pFrame.presentation_style_name.get();
				PresProps = m_Folder->GetPropertiesFromStyle(style, master);
			}

			Odt::Logic::Properties properties;
			properties = Odt::Logic::Properties::merge(GraphicProps, TextProps);
			properties = Odt::Logic::Properties::merge(PresProps, properties);

			TableConvert(pFrame.Table, pEContainer, properties, rect.left, rect.top, master);
		}
	}
}