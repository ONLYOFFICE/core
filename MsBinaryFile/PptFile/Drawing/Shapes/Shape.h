/*
 * (c) Copyright Ascensio System SIA 2010-2019
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
 * You can contact Ascensio System SIA at 20A-12 Ernesta Birznieka-Upisha
 * street, Riga, Latvia, EU, LV-1050.
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

#include "../../../Common/Vml/BaseShape.h"
#include "../../../Common/Vml/PPTXShape/PptxShape.h"
#include "../../../Common/Vml/PPTShape/PptShape.h"

/***************************************************************
вот, создавая автофигуру, или любой другой элемент
officedrawing, наследуясь от этого класса, в конструкторе
задаем все строковые параметры, и вызываем SetProperties,
которые придут, или NULL, но вызов этой функции - ОБяЗАTЕЛЕН!!!
****************************************************************/

const LONG c_ShapeDrawType_Graphic	= 0x01;
const LONG c_ShapeDrawType_Text		= 0x02;
const LONG c_ShapeDrawType_All		= c_ShapeDrawType_Graphic | c_ShapeDrawType_Text;

class CShape;
typedef boost::shared_ptr<CShape> CShapePtr;

class CShape
{
private:
	CBaseShapePtr			m_pShape;
public:
	double					m_dStartTime;
	double					m_dEndTime;

	CDoubleRect				m_rcBounds;

	long					m_lLimoX;
	long					m_lLimoY;

	PPT::CTextAttributesEx m_oText;

	double					m_dWidthLogic;
	double					m_dHeightLogic;

	double					m_dXLogic;
	double					m_dYLogic;

	/* в миллиметрах!!! */
	double					m_dTextMarginX;
	double					m_dTextMarginY;
	double					m_dTextMarginRight;
	double					m_dTextMarginBottom;

	LONG					m_lDrawType;

	std::wstring			m_strPPTXShape;
	std::wstring			m_strXmlString; //alternative

	NSBaseShape::ClassType  m_classType;

	CShape(NSBaseShape::ClassType classType, int ShapeType_) : m_rcBounds()
	{
		m_lDrawType			= c_ShapeDrawType_All;

		m_lLimoX			= 0;
		m_lLimoY			= 0;

		m_dStartTime		= 0.0;
		m_dEndTime			= 0.0;

		m_dWidthLogic		= ShapeSize;
		m_dHeightLogic		= ShapeSize;

		m_dXLogic			= 0;
		m_dYLogic			= 0;

		m_dTextMarginX		= 0;
		m_dTextMarginY		= 0;
		m_dTextMarginRight	= 0;
		m_dTextMarginBottom	= 0;

		m_classType = classType;

		if (m_classType == NSBaseShape::pptx)
		{
			m_pShape = CBaseShapePtr(new CPPTXShape());
			m_pShape->SetType(NSBaseShape::pptx, ShapeType_);
		}
		else if (m_classType == NSBaseShape::ppt)
		{
			m_pShape = CPPTShape::CreateByType((PPTShapes::ShapeType)ShapeType_ );
			if (m_pShape == NULL)
			{
				m_pShape = CBaseShapePtr(new CPPTShape());
				m_pShape->SetType(NSBaseShape::ppt, ShapeType_);
			}

			m_dTextMarginX		= 91440;
			m_dTextMarginY		= 45720;
			m_dTextMarginRight	= 91440;
			m_dTextMarginBottom	= 45720;

			m_dWidthLogic		= ShapeSizeVML;
			m_dHeightLogic		= ShapeSizeVML;
		}
	}

	~CShape()
	{
	}

	CBaseShapePtr getBaseShape()
	{
		return m_pShape;
	}

	void setBaseShape(CBaseShapePtr pShape)
	{
		CPPTXShape *pptxShape = dynamic_cast<CPPTXShape*>(pShape.get());
		CPPTShape *pptShape = dynamic_cast<CPPTShape*>(pShape.get());

		if (pptxShape)	m_classType = NSBaseShape::pptx;
		if (pptShape)	m_classType = NSBaseShape::ppt;

		m_pShape = pShape;
	}

	virtual void ReCalculate()
	{
		m_pShape->ReCalculate();
	}
	virtual void GetTextRect(CGeomShapeInfo& oInfo)
	{
		oInfo.m_dLeft	= m_dTextMarginX;
		oInfo.m_dTop	= m_dTextMarginY;
		oInfo.m_dWidth	= m_dTextMarginRight - m_dTextMarginX;
		oInfo.m_dHeight	= m_dTextMarginBottom - m_dTextMarginY;
	}

	virtual void GetTextRect(CDoubleRect& oInfo)
	{
		oInfo.left		= m_dTextMarginX;
		oInfo.top		= m_dTextMarginY;
		oInfo.right		= m_dTextMarginRight;
		oInfo.bottom	= m_dTextMarginBottom;
	}


	void ToRenderer(IRenderer* pRenderer, CGeomShapeInfo& oGeomInfo, double dStartTime, double dEndTime)
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

			m_pShape->ToRenderer(pRenderer, oGeomInfo, dStartTime, dEndTime, pen, brush);
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
		if(_T("ooxml-shape") == root.GetName())
		{
			m_pShape = CBaseShapePtr(new CPPTXShape());
			
			CPPTXShape* pptx_shape = dynamic_cast<CPPTXShape*>(m_pShape.get());
			
			return pptx_shape ? pptx_shape->LoadFromXML(root) : false;
		}
		else if(_T("shape") == root.GetName())
		{
			m_pShape = CBaseShapePtr(new CPPTShape());

			SetCoordSize(root);
			SetPen		(root);
			SetBrush	(root);
			
			CPPTShape* ppt_shape = dynamic_cast<CPPTShape*>(m_pShape.get());

			return ppt_shape ? ppt_shape->LoadFromXML(root) : false;
		}

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
			return m_pShape->SetToDublicate(Shape->m_pShape.get());
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

	void SetCoordPos(XmlUtils::CXmlNode& oNodePict)
	{
		if (_T("shape") == oNodePict.GetName())
		{
			XmlUtils::CXmlNode oNodeTemplate;
			if (oNodePict.GetNode(_T("coordorigin"), oNodeTemplate))
			{
				std::wstring strCoordSize = oNodeTemplate.GetAttributeOrValue(_T("val"));
				if (!strCoordSize.empty())
				{
					std::vector<std::wstring> oArray;
					boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

					m_dXLogic = XmlUtils::GetInteger(oArray[0]);
					m_dYLogic = XmlUtils::GetInteger(oArray[1]);
				}
			}
			else
			{
				std::wstring id = oNodePict.GetAttributeOrValue(_T("type"));
				if (id != _T(""))
				{
					m_dXLogic = 0;
					m_dYLogic = 0;
				}
				else
				{
					XmlUtils::CXmlNode oNodeTemplate;
					if (oNodePict.GetNode(_T("template"), oNodeTemplate))
					{
						std::wstring strCoordSize = oNodeTemplate.GetAttributeOrValue(_T("coordorigin"));
						if (!strCoordSize.empty())
						{
							std::vector<std::wstring> oArray;
							boost::algorithm::split(oArray, strCoordSize, boost::algorithm::is_any_of(L","), boost::algorithm::token_compress_on);

							m_dXLogic  = XmlUtils::GetInteger(oArray[0]);
							m_dYLogic = XmlUtils::GetInteger(oArray[1]);
						}
					}
				}
			}			
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
