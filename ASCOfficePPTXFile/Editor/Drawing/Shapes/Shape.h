/*
 * (c) Copyright Ascensio System SIA 2010-2017
 *
 * This program is a free software product. You can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public License (AGPL)
 * version 3 as published by the Free Software Foundation. In accordance with
 * Section 7(a) of the GNU AGPL its Section 15 shall be amended to the effect
 * that Ascensio System SIA expressly excludes the warranty of non-infringement
 * of any third-party rights.
 *
 * This program is distributed WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR  PURPOSE. For
 * details, see the GNU AGPL at: http://www.gnu.org/licenses/agpl-3.0.html
 *
 * You can contact Ascensio System SIA at Lubanas st. 125a-25, Riga, Latvia,
 * EU, LV-1021.
 *
 * The  interactive user interfaces in modified source and object code versions
 * of the Program must display Appropriate Legal Notices, as required under
 * Section 5 of the GNU AGPL version 3.
 *
 * Pursuant to Section 7(b) of the License you must retain the original Product
 * logo when distributing the program. Pursuant to Section 7(e) we decline to
 * grant you any rights under trademark law for use of our trademarks.
 *
 * All the Product's GUI elements, including illustrations and icon sets, as
 * well as technical writing content are licensed under the terms of the
 * Creative Commons Attribution-ShareAlike 4.0 International. See the License
 * terms at http://creativecommons.org/licenses/by-sa/4.0/legalcode
 *
 */
#pragma once
#include "../TextAttributesEx.h"
#include "BaseShape/BaseShape.h"

#if defined(PPTX_DEF)
#include "BaseShape/PPTXShape/PPTXShape.h"
#endif

#if defined(PPT_DEF)
#include "BaseShape/PPTShape/PPTShape.h"
#endif

using namespace NSPresentationEditor;

/***************************************************************
вот, создавая автофигуру, или любой другой элемент
officedrawing, наследуясь от этого класса, в конструкторе
задаем все строковые параметры, и вызываем SetProperties,
которые придут, или NULL, но вызов этой функции - ОБяЗАTЕЛЕН!!!
****************************************************************/

const LONG c_ShapeDrawType_Graphic	= 0x01;
const LONG c_ShapeDrawType_Text		= 0x02;
const LONG c_ShapeDrawType_All		= c_ShapeDrawType_Graphic | c_ShapeDrawType_Text;

class CShape
{
public:
	double					m_dStartTime;
	double					m_dEndTime;

	CDoubleRect				m_rcBounds;

	long					m_lLimoX;
	long					m_lLimoY;

	CTextAttributesEx		m_oText;


	double					m_dWidthLogic;
	double					m_dHeightLogic;

	/* в миллиметрах!!! */
	double					m_dTextMarginX;
	double					m_dTextMarginY;
	double					m_dTextMarginRight;
	double					m_dTextMarginBottom;

	LONG					m_lDrawType;

	CBaseShape*				m_pShape;

        std::wstring					m_strPPTXShape;
public:
	CShape(NSBaseShape::ClassType ClassType, int ShapeType_) : m_rcBounds()
	{
		m_lDrawType			= c_ShapeDrawType_All;

		m_lLimoX			= 0;
		m_lLimoY			= 0;

		m_dStartTime		= 0.0;
		m_dEndTime			= 0.0;

		m_dWidthLogic		= ShapeSize;
		m_dHeightLogic		= ShapeSize;

		m_dTextMarginX		= 0;
		m_dTextMarginY		= 0;
		m_dTextMarginRight	= 0;
		m_dTextMarginBottom	= 0;

		m_strPPTXShape = _T("");

		m_pShape = NULL;

#if defined(PPTX_DEF)
		if (ClassType == NSBaseShape::pptx)
		{
			m_pShape = new CPPTXShape();
			m_pShape->SetType(NSBaseShape::pptx, ShapeType_);
		}
#endif

#if defined(PPT_DEF)
		if (ClassType == NSBaseShape::ppt)
		{
			m_pShape = CPPTShape::CreateByType((PPTShapes::ShapeType)ShapeType_ );
			if (m_pShape == NULL)
			{
				m_pShape = new CPPTShape();
				m_pShape->SetType(NSBaseShape::ppt, ShapeType_);
			}

			m_dTextMarginX		= 2.54;
			m_dTextMarginY		= 1.27;
			m_dTextMarginRight	= 2.54;
			m_dTextMarginBottom	= 1.27;

			m_dWidthLogic		= ShapeSizeVML;
			m_dHeightLogic		= ShapeSizeVML;
		}
#endif

	}

	~CShape()
	{
		RELEASEOBJECT(m_pShape);
	}

	virtual void ReCalculate()
	{
		m_pShape->ReCalculate();
	}


        //virtual std::wstring GetTextXml(CGeomShapeInfo& oGeomInfo, CMetricInfo& pInfo, double dStartTime, double dEndTime, CTheme* pTheme, CLayout* pLayout)
	//{
	//	if (m_oText.IsEmptyText())
	//		return _T("");

	//	GetTextRect(oGeomInfo);
	//	return m_oText.ToString(oGeomInfo, pInfo, dStartTime, dEndTime, pTheme, pLayout);
	//}


        //virtual std::wstring GetBrushXml()
	//{
	//	if (!m_pShape->m_bConcentricFill)
	//		return _T("");
	//	return m_oBrush.ToString();
	//}

        //virtual std::wstring GetPenXml()
	//{
	//	return m_oPen.ToString();
	//}

	virtual void GetTextRect(CGeomShapeInfo& oInfo)
	{
		// пока сделаем типо - заглушку
		// здесь - пересчет координат, у нас пока textrect = bounds
		// поэтому пока ничего не меняем...

		bool bIsFound = false;

#ifdef PPT_DEF
		// не очень удобно мне пересчет ректов вести
		// сделаю так, отдельным методом в ппт
		double dPercentLeft		= 0;
		double dPercentTop		= 0;
		double dPercentRight	= 0;
		double dPercentBottom	= 0;

		if (NSBaseShape::ppt == m_pShape->GetClassType())
		{
			// как будто могло быть иначе
			CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(m_pShape);
			if (NULL != pPPTShape)
			{
				pPPTShape->CalcTextRectOffsets(dPercentLeft, dPercentTop, dPercentRight, dPercentBottom);

				oInfo.m_dLeft	+= (dPercentLeft * oInfo.m_dWidth);
				oInfo.m_dTop	+= (dPercentTop * oInfo.m_dHeight);

				oInfo.m_dWidth	-= ((dPercentLeft + dPercentRight) * oInfo.m_dWidth);
				oInfo.m_dHeight	-= ((dPercentTop + dPercentBottom) * oInfo.m_dHeight);
			}
		}

		// только учтем маргины
		oInfo.m_dLeft	+= m_dTextMarginX;
		oInfo.m_dTop	+= m_dTextMarginY;
		oInfo.m_dWidth  -= (m_dTextMarginX + m_dTextMarginRight);
		oInfo.m_dHeight -= (m_dTextMarginY + m_dTextMarginBottom);	

		bIsFound = true;
#endif
#ifdef PPTX_DEF

		if (!bIsFound)
		{
			if (0 < m_pShape->m_arTextRects.size())
			{
                double koef = (std::max)(oInfo.m_dWidth, oInfo.m_dHeight)/ShapeSize;
				oInfo.m_dLeft += m_pShape->m_arTextRects[0].left * koef;
				oInfo.m_dTop += m_pShape->m_arTextRects[0].top * koef;
				oInfo.m_dWidth = (m_pShape->m_arTextRects[0].right - m_pShape->m_arTextRects[0].left) * koef;
				oInfo.m_dHeight = (m_pShape->m_arTextRects[0].bottom - m_pShape->m_arTextRects[0].top) * koef;

				oInfo.m_dLeft	+= m_dTextMarginX;
				oInfo.m_dTop	+= m_dTextMarginY;
				oInfo.m_dWidth  -= (m_dTextMarginX + m_dTextMarginRight);
				oInfo.m_dHeight -= (m_dTextMarginY + m_dTextMarginBottom);
			}

			bIsFound = true;
		}
#endif


		if (oInfo.m_dWidth < 0)
			oInfo.m_dWidth = 1;
		if (oInfo.m_dHeight < 0)
			oInfo.m_dHeight = 1;
	}

	virtual void GetTextRect(CDoubleRect& oInfo)
	{
		// пока сделаем типо - заглушку
		// здесь - пересчет координат, у нас пока textrect = bounds
		// поэтому пока ничего не меняем...

		double dLeft	= oInfo.left;
		double dTop		= oInfo.top;
		double dWidth	= oInfo.GetWidth();
		double dHeight	= oInfo.GetHeight();

		bool bIsFound = false;

#ifdef PPT_DEF
		// не очень удобно мне пересчет ректов вести
		// сделаю так, отдельным методом в ппт
		double dPercentLeft		= 0;
		double dPercentTop		= 0;
		double dPercentRight	= 0;
		double dPercentBottom	= 0;

		if ((m_pShape) && (NSBaseShape::ppt == m_pShape->GetClassType()))
		{
			// как будто могло быть иначе
			CPPTShape* pPPTShape = dynamic_cast<CPPTShape*>(m_pShape);
			if (NULL != pPPTShape)
			{
				//pPPTShape->CalcTextRectOffsets(dPercentLeft, dPercentTop, dPercentRight, dPercentBottom);

				//dLeft	+= (dPercentLeft * dWidth);
				//dTop	+= (dPercentTop * dHeight);

				//dWidth	-= ((dPercentLeft + dPercentRight) * dWidth);
				//dHeight	-= ((dPercentTop + dPercentBottom) * dHeight);
			}
		}

		// только учтем маргины
		dLeft	+= m_dTextMarginX;
		dTop	+= m_dTextMarginY;
		dWidth  -= (m_dTextMarginX + m_dTextMarginRight);
		dHeight -= (m_dTextMarginY + m_dTextMarginBottom);	

		bIsFound = true;
#endif
#ifdef PPTX_DEF

		if (!bIsFound && m_pShape)
		{
			if (0 < m_pShape->m_arTextRects.size())
			{
                double koef = (std::max)(dWidth, dHeight)/ShapeSize;
				dLeft	+= m_pShape->m_arTextRects[0].left * koef;
				dTop	+= m_pShape->m_arTextRects[0].top * koef;
				dWidth	= (m_pShape->m_arTextRects[0].right - m_pShape->m_arTextRects[0].left) * koef;
				dHeight = (m_pShape->m_arTextRects[0].bottom - m_pShape->m_arTextRects[0].top) * koef;

				dLeft	+= m_dTextMarginX;
				dTop	+= m_dTextMarginY;
				dWidth  -= (m_dTextMarginX + m_dTextMarginRight);
				dHeight -= (m_dTextMarginY + m_dTextMarginBottom);
			}

			bIsFound = true;
		}
#endif


		if (dWidth < 0)
			dWidth = 1;
		if (dHeight < 0)
			dHeight = 1;
		
		oInfo.left		= dLeft;
		oInfo.top		= dTop;
		oInfo.right		= dLeft + dWidth;
		oInfo.bottom	= dTop + dHeight;
	}

        //virtual std::wstring ToXml(CGeomShapeInfo& oGeomInfo, CMetricInfo& pInfo, double dStartTime, double dEndTime, CTheme* pTheme, CLayout* pLayout)
	//{
        //	std::wstring strImageTransform = _T("");

	//	oGeomInfo.m_dLimoX = m_lLimoX;
	//	oGeomInfo.m_dLimoY = m_lLimoY;

	//	m_pShape->m_oPath.SetCoordsize((LONG)m_dWidthLogic, (LONG)m_dHeightLogic);
	//	
	//	CBrush	brush; //копии с уровня выше нужны
	//	CPen	pen;
        //	std::wstring strDrawing = m_pShape->ToXML(oGeomInfo, pInfo, dStartTime, dEndTime, brush, pen);
	//	if (m_lDrawType & c_ShapeDrawType_Graphic)
	//	{
	//		strImageTransform += strDrawing;
	//	}
	//	if (m_lDrawType & c_ShapeDrawType_Text)
	//	{
	//		strImageTransform += GetTextXml(oGeomInfo, pInfo, dStartTime, dEndTime, pTheme, pLayout);
	//	}

	//	return strImageTransform;
	//}

	void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& oGeomInfo, CMetricInfo& pInfo, double dStartTime, double dEndTime)
	{
		if (m_pShape == NULL) 
			return;
		oGeomInfo.m_dLimoX = m_lLimoX;
		oGeomInfo.m_dLimoY = m_lLimoY;

		m_pShape->m_oPath.SetCoordsize((LONG)m_dWidthLogic, (LONG)m_dHeightLogic);
		
		if (m_lDrawType & c_ShapeDrawType_Graphic)
		{
			//m_oPen.SetToRenderer(pRenderer);
			//m_oBrush.SetToRenderer(pRenderer);
			//m_oShadow.SetToRenderer(pRenderer);
			CBrush	brush; //копии с уровня выше нужны
			CPen	pen;

			m_pShape->ToRenderer(pRenderer, oGeomInfo, dStartTime, dEndTime, pen, brush, pInfo);
		}
	}


        virtual bool LoadFromXML(const std::wstring& xml)
	{
		XmlUtils::CXmlNode oNodePict;
		if (oNodePict.FromXmlString(xml))
		{
			return LoadFromXML(oNodePict);
		}
		return false;
	}

	virtual bool LoadFromXML(XmlUtils::CXmlNode& root)
	{
#if defined(PPTX_DEF)
		if(_T("ooxml-shape") == root.GetName())
		{
			if(m_pShape != NULL)
				delete m_pShape;
			m_pShape = new CPPTXShape();
			//return m_pShape->LoadFromXML(xml);
			return ((CPPTXShape*)m_pShape)->LoadFromXML(root);
		}
#endif

#if defined(PPT_DEF)
		if(_T("shape") == root.GetName())
		{
			if(m_pShape != NULL)
				delete m_pShape;
			m_pShape = new CPPTShape();

			SetCoordSize(root);
			SetPen		(root);
			SetBrush	(root);
			
			//return m_pShape->LoadFromXML(xml);
			return ((CPPTShape*)m_pShape)->LoadFromXML(root);			
		}
#endif

		return false;
	}

	virtual bool SetToDublicate(CShape* Shape)
	{
		if(Shape == NULL)
			return false;

		Shape->m_dStartTime			= m_dStartTime;
		Shape->m_dEndTime			= m_dEndTime;

		Shape->m_rcBounds			= m_rcBounds;

		Shape->m_lLimoX				= m_lLimoX;
		Shape->m_lLimoY				= m_lLimoY;

		Shape->m_oText				= m_oText;

		Shape->m_dWidthLogic		= m_dWidthLogic;
		Shape->m_dHeightLogic		= m_dHeightLogic;

		Shape->m_lDrawType			= m_lDrawType;

		Shape->m_dTextMarginX		= m_dTextMarginX;
		Shape->m_dTextMarginY		= m_dTextMarginY;
		Shape->m_dTextMarginRight	= m_dTextMarginRight;
		Shape->m_dTextMarginBottom	= m_dTextMarginBottom;

		if (m_pShape)
			return m_pShape->SetToDublicate(Shape->m_pShape);
		return true;
	}


	void SetPen(XmlUtils::CXmlNode& oNodePict)
	{
		XmlUtils::CXmlNode oNodeTemplate;
		if (oNodePict.GetNode(_T("stroke"), oNodeTemplate))
		{
                        std::wstring strColor = oNodeTemplate.GetAttributeOrValue(_T("strokecolor"));
			//if (strColor != _T(""))
			//	m_oPen.Color.FromString(strColor);
                        //std::wstring strSize = oNodeTemplate.GetAttributeOrValue(_T("strokeweight"));
			//if (strSize != _T(""))
			//	m_oPen.Size = XmlUtils::GetDouble(strSize);
                        //std::wstring strStroke = oNodeTemplate.GetAttributeOrValue(_T("stroked"));
			//if (strStroke != _T(""))
			//	m_oPen.Alpha = 0;

		}
		if (oNodePict.GetNode(_T("v:stroke"), oNodeTemplate))
		{
                        std::wstring strColor = oNodeTemplate.GetAttributeOrValue(_T("dashstyle"));
			//if (strColor != _T(""))
			//	m_oPen.DashStyle = XmlUtils::GetInteger(strColor);
		}		
	}

	void SetBrush(XmlUtils::CXmlNode& oNodePict)
	{
		XmlUtils::CXmlNode oNodeTemplate;
		if (oNodePict.GetNode(_T("fillcolor"), oNodeTemplate))
		{
                        std::wstring strColor = oNodeTemplate.GetAttributeOrValue(_T("val"));
			//if (strColor != _T(""))
			//	m_oBrush.Color1.FromString(strColor);				
		}
	}

	void SetCoordSize(XmlUtils::CXmlNode& oNodePict)
	{
		if (_T("shape") == oNodePict.GetName())
		{
			XmlUtils::CXmlNode oNodeTemplate;
			if (oNodePict.GetNode(_T("coordsize"), oNodeTemplate))
			{
                                std::wstring strCoordSize = oNodeTemplate.GetAttributeOrValue(_T("val"));
				if (strCoordSize != _T(""))
				{
                                        std::vector<std::wstring> oArray;
                                        boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

                                        m_dWidthLogic  = XmlUtils::GetInteger(oArray[0]);
					m_dHeightLogic = XmlUtils::GetInteger(oArray[1]);
				}
			}
			else
			{
                                std::wstring id = oNodePict.GetAttributeOrValue(_T("type"));
				if (id != _T(""))
				{
					m_dWidthLogic  = 21600;
					m_dHeightLogic = 21600;
				}
				else
				{
					XmlUtils::CXmlNode oNodeTemplate;
					if (oNodePict.GetNode(_T("template"), oNodeTemplate))
					{
                                                std::wstring strCoordSize = oNodeTemplate.GetAttributeOrValue(_T("coordsize"));
						if (strCoordSize != _T(""))
						{
                                                        std::vector<std::wstring> oArray;
                                                        boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

							m_dWidthLogic  = XmlUtils::GetInteger(oArray[0]);
							m_dHeightLogic = XmlUtils::GetInteger(oArray[1]);
						}
					}
				}
			}			
		}
	}
};
