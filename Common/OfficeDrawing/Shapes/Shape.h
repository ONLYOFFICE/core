#pragma once
#include "../Structures.h"
#include "BaseShape/BaseShape.h"

#include "../../XmlUtils.h"

#include "StringUtils.h"
#include "Baseshape\Common.h"


#if defined(PPTX_DEF)
#include "BaseShape/PPTXShape/PPTXShape.h"
#endif

#if defined(PPT_DEF)
#include "BaseShape/PPTShape/PPTShape.h"
#endif

#if defined(ODP_DEF)
#include "BaseShape/OdpShape/OdpShape.h"
#endif
using namespace NSAttributes;

class CElementsContainer;
class CProperties;
class CProperty;
/***************************************************************
вот, создава€ автофигуру, или любой другой элемент 
officedrawing, наследу€сь от этого класса, в конструкторе
задаем все строковые параметры, и вызываем SetProperties,
которые придут, или NULL, но вызов этой функции - ќЅя«ј“≈Ћ≈Ќ!!!
****************************************************************/

const LONG c_ShapeDrawType_Graphic	= 0x01;
const LONG c_ShapeDrawType_Text		= 0x02;
const LONG c_ShapeDrawType_All		= c_ShapeDrawType_Graphic | c_ShapeDrawType_Text;

class CShape
{
public:
	double m_dStartTime;
	double m_dEndTime;

	CDoubleRect m_rcBounds;

	long m_lLimoX;
	long m_lLimoY;

	CPen_ m_oPen;
	CBrush_ m_oBrush;
	CTextAttributesEx m_oText;

	double m_dWidthLogic;
	double m_dHeightLogic;

	/* в миллиметрах!!! */
	double m_dTextMarginX;
	double m_dTextMarginY;
	double m_dTextMarginRight;
	double m_dTextMarginBottom;

	LONG m_lDrawType;

	CBaseShape* m_pShape;
public:
	CShape(NSBaseShape::ClassType ClassType, int ShapeType) : m_rcBounds()
	{
		m_lDrawType = c_ShapeDrawType_All;

		m_lLimoX = 0;
		m_lLimoY = 0;

		m_dStartTime = 0.0;
		m_dEndTime = 0.0;

		m_dWidthLogic	= ShapeSize;
		m_dHeightLogic	= ShapeSize;

		m_dTextMarginX		= 0;
		m_dTextMarginY		= 0;
		m_dTextMarginRight	= 0;
		m_dTextMarginBottom	= 0;

		if(ClassType == NSBaseShape::unknown)
			m_pShape = NULL;

#if defined(PPTX_DEF)
		if(ClassType == NSBaseShape::pptx)
		{
			m_pShape = new CPPTXShape();
			m_pShape->SetType(NSBaseShape::pptx, ShapeType);
		}
#endif

#if defined(PPT_DEF)
		if(ClassType == NSBaseShape::ppt)
		{
			m_pShape = new CPPTShape();
			m_pShape->SetType(NSBaseShape::ppt, ShapeType);

			m_dTextMarginX		= 2.54;
			m_dTextMarginY		= 1.27;
			m_dTextMarginRight	= 2.54;
			m_dTextMarginBottom	= 1.27;
		}
#endif

#if defined(ODP_DEF)
		if(ClassType == NSBaseShape::odp)
		{
			m_pShape = new COdpShape();
			m_pShape->SetType(NSBaseShape::odp, ShapeType);
		}
#endif
	}

	~CShape()
	{
		if(m_pShape != NULL)
			delete m_pShape;
	}

	virtual void ReCalculate()
	{
		m_pShape->ReCalculate();
	}

	void SetProperties(CProperties* pProperties, CElementsContainer* pSlide);

	virtual CString GetTextXml(CGeomShapeInfo& oGeomInfo, CMetricInfo& pInfo, double dStartTime, double dEndTime)
	{
		if (m_oText.m_sText.GetLength() == 0)
		{
			return _T("");
		}

		GetTextRect(oGeomInfo);
		return m_oText.ToString(oGeomInfo, pInfo, dStartTime, dEndTime);
	}

	virtual CString GetBrushXml()
	{
		if (!m_pShape->m_bConcentricFill)
			return _T("");
		return m_oBrush.ToString();
	}

	virtual CString GetPenXml()
	{
		return m_oPen.ToString();
	}

	virtual void GetTextRect(CGeomShapeInfo& oInfo)
	{
		// пока сделаем типо - заглушку
		// здесь - пересчет координат, у нас пока textrect = bounds
		// поэтому пока ничего не мен€ем...

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
#endif
#ifdef PPTX_DEF
		double koef = max(oInfo.m_dWidth, oInfo.m_dHeight)/ShapeSize;
		oInfo.m_dLeft += m_pShape->m_arTextRects[0].left * koef;
		oInfo.m_dTop += m_pShape->m_arTextRects[0].top * koef;
		oInfo.m_dWidth = (m_pShape->m_arTextRects[0].right - m_pShape->m_arTextRects[0].left) * koef;
		oInfo.m_dHeight = (m_pShape->m_arTextRects[0].bottom - m_pShape->m_arTextRects[0].top) * koef;

		oInfo.m_dLeft	+= m_dTextMarginX;
		oInfo.m_dTop	+= m_dTextMarginY;
		oInfo.m_dWidth  -= (m_dTextMarginX + m_dTextMarginRight);
		oInfo.m_dHeight -= (m_dTextMarginY + m_dTextMarginBottom);
#endif
	}

	virtual CString ToXml(CGeomShapeInfo& oGeomInfo, CMetricInfo& pInfo, double dStartTime, double dEndTime)
	{
		CString strImageTransform = _T("");

		oGeomInfo.m_dLimoX = m_lLimoX;
		oGeomInfo.m_dLimoY = m_lLimoY;

		m_pShape->m_oPath.SetCoordsize((LONG)m_dWidthLogic, (LONG)m_dHeightLogic);
		
		if (m_lDrawType & c_ShapeDrawType_Graphic)
		{
			strImageTransform += m_pShape->ToXML(oGeomInfo, pInfo, dStartTime, dEndTime, m_oBrush, m_oPen);
		}
		if (m_lDrawType & c_ShapeDrawType_Text)
		{
			strImageTransform += GetTextXml(oGeomInfo, pInfo, dStartTime, dEndTime);
		}

		return strImageTransform;
	}

#ifdef PPT_DEF
	void ToRenderer(IASCRenderer* pRenderer, CGeomShapeInfo& oGeomInfo, CMetricInfo& pInfo, double dStartTime, double dEndTime)
	{
		oGeomInfo.m_dLimoX = m_lLimoX;
		oGeomInfo.m_dLimoY = m_lLimoY;

		m_pShape->m_oPath.SetCoordsize((LONG)m_dWidthLogic, (LONG)m_dHeightLogic);
		
		if (m_lDrawType & c_ShapeDrawType_Graphic)
		{
			m_pShape->m_oPath.ToRenderer(pRenderer, oGeomInfo, dStartTime, dEndTime, m_oPen, m_oBrush, pInfo);
		}
	}
#endif


	virtual bool LoadFromXML(const CString& xml)
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

			XmlUtils::CXmlNode templateNode;
			if (root.GetNode(_T("template"), templateNode)) {
				SetPen(templateNode);
				SetBrush(templateNode);
			}

			SetCoordSize(root);
			SetPen(root);
			SetBrush(root);
			
			//return m_pShape->LoadFromXML(xml);
			return ((CPPTShape*)m_pShape)->LoadFromXML(root);			
		}
#endif

#if defined(ODP_DEF)
		if(_T("draw:enhanced-geometry") == root.GetName())
		{
			if(m_pShape != NULL)
				delete m_pShape;
			m_pShape = new COdpShape();
			//return m_pShape->LoadFromXML(xml);
			return ((COdpShape*)m_pShape)->LoadFromXML(root);
		}
#endif
		return false;
	}


//	virtual bool LoadFromXML(const CString& xml)
//	{
//		XmlUtils::CXmlNode root;
//		if(root.FromXmlString(xml))
//		{
//#if defined(PPTX_DEF)
//			if(_T("ooxml-shape") == root.GetName())
//			{
//				if(m_pShape != NULL)
//					delete m_pShape;
//				m_pShape = new CPPTXShape();
//				//return m_pShape->LoadFromXML(xml);
//				return ((CPPTXShape*)m_pShape)->LoadFromXML(root);
//			}
//#endif
//
//#if defined(PPT_DEF)
//			if(_T("shape") == root.GetName())
//			{
//				if(m_pShape != NULL)
//					delete m_pShape;
//				m_pShape = new CPPTShape();
//
//				SetCoordSize(root);
//				SetPen(root);
//				SetBrush(root);
//				
//				//return m_pShape->LoadFromXML(xml);
//				return ((CPPTShape*)m_pShape)->LoadFromXML(root);
//			}
//#endif
//
//#if defined(ODP_DEF)
//			if(_T("draw:enhanced-geometry") == root.GetName())
//			{
//				if(m_pShape != NULL)
//					delete m_pShape;
//				m_pShape = new COdpShape();
//				//return m_pShape->LoadFromXML(xml);
//				return ((COdpShape*)m_pShape)->LoadFromXML(root);
//			}
//#endif
//		}
//		return false;
//	}


	virtual bool SetToDublicate(CShape* Shape)
	{
		if((Shape == NULL) || (m_pShape == NULL))
			return false;

		Shape->m_dStartTime		= m_dStartTime;
		Shape->m_dEndTime		= m_dEndTime;

		Shape->m_rcBounds		= m_rcBounds;

		Shape->m_lLimoX			= m_lLimoX;
		Shape->m_lLimoY			= m_lLimoY;

		Shape->m_oPen			= m_oPen;
		Shape->m_oBrush			= m_oBrush;
		Shape->m_oText			= m_oText;

		Shape->m_dWidthLogic	= m_dWidthLogic;
		Shape->m_dHeightLogic	= m_dHeightLogic;

		Shape->m_lDrawType		= m_lDrawType;

		Shape->m_dTextMarginX		= m_dTextMarginX;
		Shape->m_dTextMarginY		= m_dTextMarginY;
		Shape->m_dTextMarginRight	= m_dTextMarginRight;
		Shape->m_dTextMarginBottom	= m_dTextMarginBottom;

		return m_pShape->SetToDublicate(Shape->m_pShape);
	}


	void SetPen(XmlUtils::CXmlNode& oNodePict)
	{
		XmlUtils::CXmlNode oNodeTemplate;
		if (oNodePict.GetNode(_T("stroke"), oNodeTemplate))
		{
			CString strColor = oNodeTemplate.GetAttributeOrValue(_T("strokecolor"));
			if (strColor != _T(""))
				m_oPen.m_oColor = getColorFromString(strColor);
			CString strSize = oNodeTemplate.GetAttributeOrValue(_T("strokeweight"));
			if (strSize != _T(""))
				m_oPen.Size = XmlUtils::GetDouble(strSize);
			CString strStroke = oNodeTemplate.GetAttributeOrValue(_T("stroked"));
			if (strStroke == _T("t"))
				m_oPen.m_nAlpha = 255;
			else if (strStroke != _T(""))
				m_oPen.m_nAlpha = 0;
		}
		if (oNodePict.GetNode(_T("v:stroke"), oNodeTemplate))
		{
			CString style = oNodeTemplate.GetAttributeOrValue(_T("dashstyle"));
			if (style == _T("dash") || style == _T("longDash"))
				m_oPen.DashStyle = DashStyleDash;
			else if (style == _T("dashDot") || style == _T("longDashDot"))
				m_oPen.DashStyle = DashStyleDashDot;
			else if (style == _T("dashDotDot") || style == _T("longDashDotDot"))
				m_oPen.DashStyle = DashStyleDashDotDot;
			else if (style == _T("1 1"))
				m_oPen.DashStyle = DashStyleDot;

			CString endarrow = oNodeTemplate.GetAttribute(_T("endarrow"));
			if (endarrow == _T("block"))
				m_oPen.LineEndCap = LineCapArrowAnchor;
		}		
	}

	void SetBrush(XmlUtils::CXmlNode& oNodePict)
	{
#ifdef _DEBUG
		CString xml = oNodePict.GetXml();
#endif
		
		if (oNodePict.GetAttribute(_T("filled")) == _T("f"))
		{
			m_oBrush.m_Alpha1 = 0;
			m_oBrush.m_Alpha2 = 0;
			return;
		}
		XmlUtils::CXmlNode oNodeTemplate;
		CString strColor;
		if (oNodePict.GetNode(_T("fillcolor"), oNodeTemplate))
		{
			strColor = oNodeTemplate.GetAttributeOrValue(_T("val"));
			if (strColor != _T(""))
					m_oBrush.m_oColor1 = getColorFromString(strColor);
		}
		if (strColor != _T("none"))
		{
			if (strColor != _T(""))
				m_oBrush.m_oColor1 = getColorFromString(strColor);
			if (oNodePict.GetNode(_T("fill"), oNodeTemplate))
			{
				CString strColor = oNodeTemplate.GetAttributeOrValue(_T("color2"));
				if (strColor != _T("") && strColor != _T("none"))
					m_oBrush.m_oColor2 = getColor2FromString(strColor, m_oBrush.m_oColor1);

				CString strOpacity1 = oNodeTemplate.GetAttributeOrValue(_T("opacity"));
				if (strOpacity1 != _T(""))
					m_oBrush.m_Alpha1 = getOpacityFromString(strOpacity1);

				CString strOpacity2 = oNodeTemplate.GetAttributeOrValue(_T("o:opacity2"));
				if (strOpacity2 != _T(""))
					m_oBrush.m_Alpha2 = getOpacityFromString(strOpacity2);

				CString focus  = oNodeTemplate.GetAttributeOrValue(_T("focus"));			

				CString typeFill = oNodeTemplate.GetAttributeOrValue(_T("type"));
				if (typeFill == _T("tile") || typeFill == _T("frame") || typeFill == _T("pattern"))
				{
					XmlUtils::CXmlNode oNodeSource;
					if (oNodeTemplate.GetNode(_T("source"), oNodeSource))
					{
						CString path = oNodeSource.GetAttributeOrValue(_T("dir")) + _T("/word/") + oNodeSource.GetText();

						if ( typeFill == _T("tile") )
						{
							m_oBrush.m_nTextureMode = BrushTextureModeTile;
							
							if (strColor != _T("") && strColor != _T("none"))
								m_oBrush.m_nBrushType = BrushTypePattern;
							else
								m_oBrush.m_nBrushType = BrushTypeTexture;
						}
						else if( typeFill == _T("pattern") )
						{
							m_oBrush.m_nTextureMode = BrushTextureModeTileCenter;
							m_oBrush.m_nBrushType = BrushTypeTexture;
						}
						else
						{
							m_oBrush.m_nTextureMode = BrushTextureModeStretch;
							m_oBrush.m_nBrushType = BrushTypeTexture;
						}
						m_oBrush.m_sTexturePath = path;
					}
				}
				else if (typeFill == _T("gradient"))
				{				
					CString rotate = oNodeTemplate.GetAttributeOrValue(_T("rotate"));
					CString angle  = oNodeTemplate.GetAttributeOrValue(_T("angle"));

					m_oBrush.m_fAngle = 90 - (float)XmlUtils::GetDouble( angle );
					m_oBrush.m_nBrushType = BrushTypeHorizontal;

					m_oBrush.m_arrSubColors.RemoveAll();

					CString subcolors = oNodeTemplate.GetAttribute(_T("colors"));
					if( !subcolors.IsEmpty() )
					{
						CBrush_::TSubColor tSubColor;

						int length = subcolors.GetLength();
						int start = 0;

						for( ;; )
						{
							CString para;
							
							int pos = subcolors.Find( _T(';'), start );
							if( pos < 0 )
							{
								if( start < length )
								{
									para = subcolors.Mid( start );
									start = length;
								}
								else
									break;
							}
							else
							{
								para = subcolors.Mid( start, pos - start );
								start = pos + 1;
							}

							if( para.IsEmpty() )
								continue;

							int t = para.Find( _T(' ') );
							if( t < 0 )
								continue;

							tSubColor.position = getRealFromString( para.Mid( 0, t ) );
							tSubColor.color = getColorFromString( para.Mid( t + 1 ) );

							m_oBrush.m_arrSubColors.Add( tSubColor );
						}
					}
				}
				else if (typeFill == _T("gradientRadial"))
				{
					m_oBrush.m_nBrushType = BrushTypeCenter;
					m_oBrush.m_fAngle = 45;
					
					m_oBrush.m_arrSubColors.RemoveAll();

					CString subcolors = oNodeTemplate.GetAttribute(_T("colors"));
					if( !subcolors.IsEmpty() )
					{
						CBrush_::TSubColor tSubColor;

						int length = subcolors.GetLength();
						int start = 0;

						for( ;; )
						{
							CString para;
							
							int pos = subcolors.Find( _T(';'), start );
							if( pos < 0 )
							{
								if( start < length )
								{
									para = subcolors.Mid( start );
									start = length;
								}
								else
									break;
							}
							else
							{
								para = subcolors.Mid( start, pos - start );
								start = pos + 1;
							}

							if( para.IsEmpty() )
								continue;

							int t = para.Find( _T(' ') );
							if( t < 0 )
								continue;

							tSubColor.position = getRealFromString( para.Mid( 0, t ) );
							tSubColor.color = getColorFromString( para.Mid( t + 1 ) );

							m_oBrush.m_arrSubColors.Add( tSubColor );
						}
					}
				}		
				else
				{
					m_oBrush.m_nBrushType = BrushTypeSolid;
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
				CString strCoordSize = oNodeTemplate.GetAttributeOrValue(_T("val"));
				if (strCoordSize != _T(""))
				{
					CSimpleArray<CString> oArray;
					NSStringUtils::ParseString(_T(","), strCoordSize, &oArray);
					
					LONG lCountSizes = oArray.GetSize();
					if (2 <= lCountSizes)
					{
						m_dWidthLogic  = XmlUtils::GetInteger(oArray[0]);
						m_dHeightLogic = XmlUtils::GetInteger(oArray[1]);
					}
					else if (1 == lCountSizes)
					{
						m_dWidthLogic  = XmlUtils::GetInteger(oArray[0]);
						m_dHeightLogic = m_dWidthLogic;
					}
					else
					{
						m_dWidthLogic  = 21600;
						m_dHeightLogic = 21600;
					}
				}
			}
			else
			{
				CString id = oNodePict.GetAttributeOrValue(_T("type"));
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
						CString strCoordSize = oNodeTemplate.GetAttributeOrValue(_T("coordsize"));
						if (strCoordSize != _T(""))
						{
							CSimpleArray<CString> oArray;
							NSStringUtils::ParseString(_T(","), strCoordSize, &oArray);
							m_dWidthLogic  = XmlUtils::GetInteger(oArray[0]);
							m_dHeightLogic = XmlUtils::GetInteger(oArray[1]);
						}
					}
				}
			}			
		}
	}


	CColor_ getColorFromString(const CString colorStr)
	{
		CColor_ color;
		int lColor;
		if (colorStr.Find(_T("fill darken")) != -1)
		{
			CString p = colorStr.Mid(colorStr.Find(_T("(")) + 1, colorStr.Find(_T(")")) - colorStr.Find(_T("(")) - 1);
			int c = XmlUtils::GetInteger(colorStr.Mid(colorStr.Find(_T("(")) + 1, colorStr.Find(_T(")")) - colorStr.Find(_T("(")) - 1));
			
		}
		else if (colorStr.Find(_T("#")) != -1)
		{
			lColor = XmlUtils::GetColor(colorStr.Mid(1, 6));
			color.R = (BYTE)(lColor >> 0);
			color.G = (BYTE)(lColor >> 8);
			color.B = (BYTE)(lColor >> 16);
			color.A = 0;
		}
		else
		{
			CString str;

			int pos = colorStr.Find(' ');
			if( pos < 0 )
				str = colorStr;
			else
				str = colorStr.Left( pos );

			int RGB = 0;

			switch(str[0])
			{
			case 'a':
				if(str == "aliceBlue")			{RGB = 0xF0F8FF; break;} // (Alice Blue Preset Color)		Specifies a color with RGB value (240,248,255) 
				if(str == "antiqueWhite")		{RGB = 0xFAEBD7; break;} // (Antique White Preset Color)	Specifies a color with RGB value (250,235,215) 
				if(str == "aqua")				{RGB = 0x00FFFF; break;} // (Aqua Preset Color)				Specifies a color with RGB value (0,255,255) 
				if(str == "aquamarine")			{RGB = 0x7FFFD4; break;} // (Aquamarine Preset Color)		Specifies a color with RGB value (127,255,212) 
				if(str == "azure")				{RGB = 0xF0FFFF; break;} // (Azure Preset Color)			Specifies a color with RGB value (240,255,255) 
				break;
			case 'b':
				if(str == "beige")				{RGB = 0xF5F5DC; break;} // (Beige Preset Color)			Specifies a color with RGB value (245,245,220) 
				if(str == "bisque")				{RGB = 0xFFE4C4; break;} // (Bisque Preset Color)			Specifies a color with RGB value (255,228,196) 
				if(str == "black")				{RGB = 0x000000; break;} // (Black Preset Color)			Specifies a color with RGB value (0,0,0) 
				if(str == "blanchedAlmond")		{RGB = 0xFFEBCD; break;} // (Blanched Almond Preset Color)  Specifies a color with RGB value (255,235,205) 
				if(str == "blue")				{RGB = 0x0000FF; break;} // (Blue Preset Color)				Specifies a color with RGB value (0,0,255) 
				if(str == "blueViolet")			{RGB = 0x8A2BE2; break;} // (Blue Violet Preset Color)		Specifies a color with RGB value (138,43,226) 
				if(str == "brown")				{RGB = 0xA52A2A; break;} // (Brown Preset Color)			Specifies a color with RGB value (165,42,42) 
				if(str == "burlyWood")			{RGB = 0xDEB887; break;} // (Burly Wood Preset Color)		Specifies a color with RGB value (222,184,135) 
				break;
			case 'c':
				if(str == "cadetBlue")			{RGB = 0x5F9EA0; break;} // (Cadet Blue Preset Color)		Specifies a color with RGB value (95,158,160) 
				if(str == "chartreuse")			{RGB = 0x7FFF00; break;} // (Chartreuse Preset Color)		Specifies a color with RGB value (127,255,0) 
				if(str == "chocolate")			{RGB = 0xD2691E; break;} // (Chocolate Preset Color)		Specifies a color with RGB value (210,105,30) 
				if(str == "coral")				{RGB = 0xFF7F50; break;} // (Coral Preset Color)			Specifies a color with RGB value (255,127,80) 
				if(str == "cornflowerBlue")		{RGB = 0x6495ED; break;} // (Cornflower Blue Preset Color)  Specifies a color with RGB value (100,149,237) 
				if(str == "cornsilk")			{RGB = 0xFFF8DC; break;} // (Cornsilk Preset Color)			Specifies a color with RGB value (255,248,220) 
				if(str == "crimson")			{RGB = 0xDC143C; break;} // (Crimson Preset Color)			Specifies a color with RGB value (220,20,60) 
				if(str == "cyan")				{RGB = 0x00FFFF; break;} // (Cyan Preset Color)				Specifies a color with RGB value (0,255,255) 
				break;
			case 'd':
				if(str == "darkBlue")			{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139) 
				if(str == "darkCyan")			{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139) 
				if(str == "darkGoldenrod")		{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11) 
				if(str == "darkGray")			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
				if(str == "darkGreen")			{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0) 
				if(str == "darkGrey")			{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
				if(str == "darkKhaki")			{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107) 
				if(str == "darkMagenta")		{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139) 
				if(str == "darkOliveGreen")		{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47) 
				if(str == "darkOrange")			{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0) 
				if(str == "darkOrchid")			{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204) 
				if(str == "darkRed")			{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0) 
				if(str == "darkSalmon")			{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122) 
				if(str == "darkSeaGreen")		{RGB = 0x8FBC8F; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,143) 
				if(str == "darkSlateBlue")		{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139) 
				if(str == "darkSlateGray")		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
				if(str == "darkSlateGrey")		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
				if(str == "darkTurquoise")		{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209) 
				if(str == "darkViolet")			{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211) 
				if(str == "deepPink")			{RGB = 0xFF1493; break;} // (Deep Pink Preset Color)		Specifies a color with RGB value (255,20,147) 
				if(str == "deepSkyBlue")		{RGB = 0x00BFFF; break;} // (Deep Sky Blue Preset Color)	Specifies a color with RGB value (0,191,255) 
				if(str == "dimGray")			{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105) 
				if(str == "dimGrey")			{RGB = 0x696969; break;} // (Dim Gray Preset Color)			Specifies a color with RGB value (105,105,105) 
				if(str == "dkBlue")				{RGB = 0x00008B; break;} // (Dark Blue Preset Color)		Specifies a color with RGB value (0,0,139) 
				if(str == "dkCyan")				{RGB = 0x008B8B; break;} // (Dark Cyan Preset Color)		Specifies a color with RGB value (0,139,139) 
				if(str == "dkGoldenrod")		{RGB = 0xB8860B; break;} // (Dark Goldenrod Preset Color)	Specifies a color with RGB value (184,134,11) 
				if(str == "dkGray")				{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
				if(str == "dkGreen")			{RGB = 0x006400; break;} // (Dark Green Preset Color)		Specifies a color with RGB value (0,100,0) 
				if(str == "dkGrey")				{RGB = 0xA9A9A9; break;} // (Dark Gray Preset Color)		Specifies a color with RGB value (169,169,169) 
				if(str == "dkKhaki")			{RGB = 0xBDB76B; break;} // (Dark Khaki Preset Color)		Specifies a color with RGB value (189,183,107) 
				if(str == "dkMagenta")			{RGB = 0x8B008B; break;} // (Dark Magenta Preset Color)		Specifies a color with RGB value (139,0,139) 
				if(str == "dkOliveGreen")		{RGB = 0x556B2F; break;} // (Dark Olive Green Preset Color) Specifies a color with RGB value (85,107,47) 
				if(str == "dkOrange")			{RGB = 0xFF8C00; break;} // (Dark Orange Preset Color)		Specifies a color with RGB value (255,140,0) 
				if(str == "dkOrchid")			{RGB = 0x9932CC; break;} // (Dark Orchid Preset Color)		Specifies a color with RGB value (153,50,204) 
				if(str == "dkRed")				{RGB = 0x8B0000; break;} // (Dark Red Preset Color)			Specifies a color with RGB value (139,0,0) 
				if(str == "dkSalmon")			{RGB = 0xE9967A; break;} // (Dark Salmon Preset Color)		Specifies a color with RGB value (233,150,122) 
				if(str == "dkSeaGreen")			{RGB = 0x8FBC8B; break;} // (Dark Sea Green Preset Color)	Specifies a color with RGB value (143,188,139) 
				if(str == "dkSlateBlue")		{RGB = 0x483D8B; break;} // (Dark Slate Blue Preset Color)  Specifies a color with RGB value (72,61,139) 
				if(str == "dkSlateGray")		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
				if(str == "dkSlateGrey")		{RGB = 0x2F4F4F; break;} // (Dark Slate Gray Preset Color)  Specifies a color with RGB value (47,79,79) 
				if(str == "dkTurquoise")		{RGB = 0x00CED1; break;} // (Dark Turquoise Preset Color)	Specifies a color with RGB value (0,206,209) 
				if(str == "dkViolet")			{RGB = 0x9400D3; break;} // (Dark Violet Preset Color)		Specifies a color with RGB value (148,0,211) 
				if(str == "dodgerBlue")			{RGB = 0x1E90FF; break;} // (Dodger Blue Preset Color)		Specifies a color with RGB value (30,144,255) 
				break;
			case 'f':
				if(str == "firebrick")			{RGB = 0xB22222; break;} // (Firebrick Preset Color)		Specifies a color with RGB value (178,34,34) 
				if(str == "floralWhite")		{RGB = 0xFFFAF0; break;} // (Floral White Preset Color)		Specifies a color with RGB value (255,250,240) 
				if(str == "forestGreen")		{RGB = 0x228B22; break;} // (Forest Green Preset Color)		Specifies a color with RGB value (34,139,34) 
				if(str == "fuchsia")			{RGB = 0xFF00FF; break;} // (Fuchsia Preset Color)			Specifies a color with RGB value (255,0,255) 
				break;
			case 'g':
				if(str == "gainsboro")			{RGB = 0xDCDCDC; break;} // (Gainsboro Preset Color)		Specifies a color with RGB value (220,220,220) 
				if(str == "ghostWhite")			{RGB = 0xF8F8FF; break;} // (Ghost White Preset Color)		Specifies a color with RGB value (248,248,255) 
				if(str == "gold")				{RGB = 0xFFD700; break;} // (Gold Preset Color)				Specifies a color with RGB value (255,215,0) 
				if(str == "goldenrod")			{RGB = 0xDAA520; break;} // (Goldenrod Preset Color)		Specifies a color with RGB value (218,165,32) 
				if(str == "gray")				{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128) 
				if(str == "green")				{RGB = 0x008000; break;} // (Green Preset Color)				Specifies a color with RGB value (0,128,0) 
				if(str == "greenYellow")		{RGB = 0xADFF2F; break;} // (Green Yellow Preset Color)		Specifies a color with RGB value (173,255,47) 
				if(str == "grey")				{RGB = 0x808080; break;} // (Gray Preset Color)				Specifies a color with RGB value (128,128,128) 
				break;
			case 'h':
				if(str == "honeydew")			{RGB = 0xF0FFF0; break;} // (Honeydew Preset Color)			Specifies a color with RGB value (240,255,240) 
				if(str == "hotPink")			{RGB = 0xFF69B4; break;} // (Hot Pink Preset Color)			Specifies a color with RGB value (255,105,180) 
				break;
			case 'i':
				if(str == "indianRed")			{RGB = 0xCD5C5C; break;} // (Indian Red Preset Color)		Specifies a color with RGB value (205,92,92) 
				if(str == "indigo")				{RGB = 0x4B0082; break;} // (Indigo Preset Color)			Specifies a color with RGB value (75,0,130) 
				if(str == "ivory")				{RGB = 0xFFFFF0; break;} // (Ivory Preset Color)			Specifies a color with RGB value (255,255,240) 
				break;
			case 'k':
				if(str == "khaki")				{RGB = 0xF0E68C; break;} // (Khaki Preset Color)			Specifies a color with RGB value (240,230,140) 
				break;
			case 'l':
				if(str == "lavender")			{RGB = 0xE6E6FA; break;} // (Lavender Preset Color)			Specifies a color with RGB value (230,230,250) 
				if(str == "lavenderBlush")		{RGB = 0xFFF0F5; break;} // (Lavender Blush Preset Color)	Specifies a color with RGB value (255,240,245) 
				if(str == "lawnGreen")			{RGB = 0x7CFC00; break;} // (Lawn Green Preset Color)		Specifies a color with RGB value (124,252,0) 
				if(str == "lemonChiffon")		{RGB = 0xFFFACD; break;} // (Lemon Chiffon Preset Color)	Specifies a color with RGB value (255,250,205) 
				if(str == "lightBlue")			{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230) 
				if(str == "lightCoral")			{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128) 
				if(str == "lightCyan")			{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255) 
				if(str=="lightGoldenrodYellow")	{RGB = 0xFAFAD2;break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,210) 
				if(str == "lightGray")			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
				if(str == "lightGreen")			{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144) 
				if(str == "lightGrey")			{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
				if(str == "lightPink")			{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193) 
				if(str == "lightSalmon")		{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122) 
				if(str == "lightSeaGreen")		{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170) 
				if(str == "lightSkyBlue")		{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250) 
				if(str == "lightSlateGray")		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
				if(str == "lightSlateGrey")		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
				if(str == "lightSteelBlue")		{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222) 
				if(str == "lightYellow")		{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224) 
				if(str == "lime")				{RGB = 0x00FF00; break;} // (Lime Preset Color)				Specifies a color with RGB value (0,255,0) 
				if(str == "limeGreen")			{RGB = 0x32CD32; break;} // (Lime Green Preset Color)		Specifies a color with RGB value (50,205,50) 
				if(str == "linen")				{RGB = 0xFAF0E6; break;} // (Linen Preset Color)			Specifies a color with RGB value (250,240,230) 
				if(str == "ltBlue")				{RGB = 0xADD8E6; break;} // (Light Blue Preset Color)		Specifies a color with RGB value (173,216,230) 
				if(str == "ltCoral")			{RGB = 0xF08080; break;} // (Light Coral Preset Color)		Specifies a color with RGB value (240,128,128) 
				if(str == "ltCyan")				{RGB = 0xE0FFFF; break;} // (Light Cyan Preset Color)		Specifies a color with RGB value (224,255,255) 
				if(str == "ltGoldenrodYellow")	{RGB = 0xFAFA78; break;} // (Light Goldenrod Color)			Specifies a color with RGB value (250,250,120) 
				if(str == "ltGray")				{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
				if(str == "ltGreen")			{RGB = 0x90EE90; break;} // (Light Green Preset Color)		Specifies a color with RGB value (144,238,144) 
				if(str == "ltGrey")				{RGB = 0xD3D3D3; break;} // (Light Gray Preset Color)		Specifies a color with RGB value (211,211,211) 
				if(str == "ltPink")				{RGB = 0xFFB6C1; break;} // (Light Pink Preset Color)		Specifies a color with RGB value (255,182,193) 
				if(str == "ltSalmon")			{RGB = 0xFFA07A; break;} // (Light Salmon Preset Color)		Specifies a color with RGB value (255,160,122) 
				if(str == "ltSeaGreen")			{RGB = 0x20B2AA; break;} // (Light Sea Green Preset Color)  Specifies a color with RGB value (32,178,170) 
				if(str == "ltSkyBlue")			{RGB = 0x87CEFA; break;} // (Light Sky Blue Preset Color)	Specifies a color with RGB value (135,206,250) 
				if(str == "ltSlateGray")		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
				if(str == "ltSlateGrey")		{RGB = 0x778899; break;} // (Light Slate Gray Preset Color) Specifies a color with RGB value (119,136,153) 
				if(str == "ltSteelBlue")		{RGB = 0xB0C4DE; break;} // (Light Steel Blue Preset Color) Specifies a color with RGB value (176,196,222) 
				if(str == "ltYellow")			{RGB = 0xFFFFE0; break;} // (Light Yellow Preset Color)		Specifies a color with RGB value (255,255,224) 
				break;
			case 'm':
				if(str == "magenta")			{RGB = 0xFF00FF; break;} // (Magenta Preset Color)			Specifies a color with RGB value (255,0,255) 
				if(str == "maroon")				{RGB = 0x800000; break;} // (Maroon Preset Color)			Specifies a color with RGB value (128,0,0) 
				if(str == "medAquamarine")		{RGB = 0x66CDAA; break;} // (Medium Aquamarine Preset Color)Specifies a color with RGB value (102,205,170) 
				if(str == "medBlue")			{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205) 
				if(str == "mediumAquamarine")	{RGB = 0x66CDAA; break;} // (Medium Aquamarine Color)		Specifies a color with RGB value (102,205,170) 
				if(str == "mediumBlue")			{RGB = 0x0000CD; break;} // (Medium Blue Preset Color)		Specifies a color with RGB value (0,0,205) 
				if(str == "mediumOrchid")		{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211) 
				if(str == "mediumPurple")		{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219) 
				if(str == "mediumSeaGreen")		{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113) 
				if(str == "mediumSlateBlue")	{RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238) 
				if(str == "mediumSpringGreen")	{RGB = 0x00FA9A; break;} // (Medium Spring Color)			Specifies a color with RGB value (0,250,154) 
				if(str == "mediumTurquoise")	{RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204) 
				if(str == "mediumVioletRed")	{RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133) 
				if(str == "medOrchid")			{RGB = 0xBA55D3; break;} // (Medium Orchid Preset Color)	Specifies a color with RGB value (186,85,211) 
				if(str == "medPurple")			{RGB = 0x9370DB; break;} // (Medium Purple Preset Color)	Specifies a color with RGB value (147,112,219) 
				if(str == "medSeaGreen")		{RGB = 0x3CB371; break;} // (Medium Sea Green Preset Color) Specifies a color with RGB value (60,179,113) 
				if(str == "medSlateBlue")		{RGB = 0x7B68EE; break;} // (Medium Slate Blue Preset Color)Specifies a color with RGB value (123,104,238) 
				if(str == "medSpringGreen")		{RGB = 0x00FA9A; break;} // (Medium Spring Preset Color)	Specifies a color with RGB value (0,250,154) 
				if(str == "medTurquoise")		{RGB = 0x48D1CC; break;} // (Medium Turquoise Preset Color) Specifies a color with RGB value (72,209,204) 
				if(str == "medVioletRed")		{RGB = 0xC71585; break;} // (Medium Violet Red Preset Color)Specifies a color with RGB value (199,21,133) 
				if(str == "midnightBlue")		{RGB = 0x191970; break;} // (Midnight Blue Preset Color)	Specifies a color with RGB value (25,25,112) 
				if(str == "mintCream")			{RGB = 0xF5FFFA; break;} // (Mint Cream Preset Color)		Specifies a color with RGB value (245,255,250) 
				if(str == "mistyRose")			{RGB = 0xFFE4FF; break;} // (Misty Rose Preset Color)		Specifies a color with RGB value (255,228,225) 
				if(str == "moccasin")			{RGB = 0xFFE4B5; break;} // (Moccasin Preset Color)			Specifies a color with RGB value (255,228,181) 
				break;
			case 'n':
				if(str == "navajoWhite")		{RGB = 0xFFDEAD; break;} // (Navajo White Preset Color)		Specifies a color with RGB value (255,222,173) 
				if(str == "navy")				{RGB = 0x000080; break;} // (Navy Preset Color)				Specifies a color with RGB value (0,0,128) 
				break;
			case 'o':
				if(str == "oldLace")			{RGB = 0xFDF5E6; break;} // (Old Lace Preset Color)			Specifies a color with RGB value (253,245,230) 
				if(str == "olive")				{RGB = 0x808000; break;} // (Olive Preset Color)			Specifies a color with RGB value (128,128,0) 
				if(str == "oliveDrab")			{RGB = 0x6B8E23; break;} // (Olive Drab Preset Color)		Specifies a color with RGB value (107,142,35) 
				if(str == "orange")				{RGB = 0xFFA500; break;} // (Orange Preset Color)			Specifies a color with RGB value (255,165,0) 
				if(str == "orangeRed")			{RGB = 0xFF4500; break;} // (Orange Red Preset Color)		Specifies a color with RGB value (255,69,0) 
				if(str == "orchid")				{RGB = 0xDA70D6; break;} // (Orchid Preset Color)			Specifies a color with RGB value (218,112,214) 
				break;
			case 'p':
				if(str == "paleGoldenrod")		{RGB = 0xEEE8AA; break;} // (Pale Goldenrod Preset Color)	Specifies a color with RGB value (238,232,170) 
				if(str == "paleGreen")			{RGB = 0x98FB98; break;} // (Pale Green Preset Color)		Specifies a color with RGB value (152,251,152) 
				if(str == "paleTurquoise")		{RGB = 0xAFEEEE; break;} // (Pale Turquoise Preset Color)	Specifies a color with RGB value (175,238,238) 
				if(str == "paleVioletRed")		{RGB = 0xDB7093; break;} // (Pale Violet Red Preset Color)  Specifies a color with RGB value (219,112,147) 
				if(str == "papayaWhip")			{RGB = 0xFFEFD5; break;} // (Papaya Whip Preset Color)		Specifies a color with RGB value (255,239,213) 
				if(str == "peachPuff")			{RGB = 0xFFDAB9; break;} // (Peach Puff Preset Color)		Specifies a color with RGB value (255,218,185) 
				if(str == "peru")				{RGB = 0xCD853F; break;} // (Peru Preset Color)				Specifies a color with RGB value (205,133,63) 
				if(str == "pink")				{RGB = 0xFFC0CB; break;} // (Pink Preset Color)				Specifies a color with RGB value (255,192,203) 
				if(str == "plum")				{RGB = 0xD3A0D3; break;} // (Plum Preset Color)				Specifies a color with RGB value (221,160,221) 
				if(str == "powderBlue")			{RGB = 0xB0E0E6; break;} // (Powder Blue Preset Color)		Specifies a color with RGB value (176,224,230) 
				if(str == "purple")				{RGB = 0x800080; break;} // (Purple Preset Color)			Specifies a color with RGB value (128,0,128) 
				break;
			case 'r':
				if(str == "red")				{RGB = 0xFF0000; break;} // (Red Preset Color)				Specifies a color with RGB value (255,0,0) 
				if(str == "rosyBrown")			{RGB = 0xBC8F8F; break;} // (Rosy Brown Preset Color)		Specifies a color with RGB value (188,143,143) 
				if(str == "royalBlue")			{RGB = 0x4169E1; break;} // (Royal Blue Preset Color)		Specifies a color with RGB value (65,105,225) 
				break;
			case 's':
				if(str == "saddleBrown")		{RGB = 0x8B4513; break;} // (Saddle Brown Preset Color)		Specifies a color with RGB value (139,69,19) 
				if(str == "salmon")				{RGB = 0xFA8072; break;} // (Salmon Preset Color)			Specifies a color with RGB value (250,128,114) 
				if(str == "sandyBrown")			{RGB = 0xF4A460; break;} // (Sandy Brown Preset Color)		Specifies a color with RGB value (244,164,96) 
				if(str == "seaGreen")			{RGB = 0x2E8B57; break;} // (Sea Green Preset Color)		Specifies a color with RGB value (46,139,87) 
				if(str == "seaShell")			{RGB = 0xFFF5EE; break;} // (Sea Shell Preset Color)		Specifies a color with RGB value (255,245,238) 
				if(str == "sienna")				{RGB = 0xA0522D; break;} // (Sienna Preset Color)			Specifies a color with RGB value (160,82,45) 
				if(str == "silver")				{RGB = 0xC0C0C0; break;} // (Silver Preset Color)			Specifies a color with RGB value (192,192,192) 
				if(str == "skyBlue")			{RGB = 0x87CEEB; break;} // (Sky Blue Preset Color)			Specifies a color with RGB value (135,206,235) 
				if(str == "slateBlue")			{RGB = 0x6A5AEB; break;} // (Slate Blue Preset Color)		Specifies a color with RGB value (106,90,205) 
				if(str == "slateGray")			{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144) 
				if(str == "slateGrey")			{RGB = 0x708090; break;} // (Slate Gray Preset Color)		Specifies a color with RGB value (112,128,144) 
				if(str == "snow")				{RGB = 0xFFFAFA; break;} // (Snow Preset Color)				Specifies a color with RGB value (255,250,250) 
				if(str == "springGreen")		{RGB = 0x00FF7F; break;} // (Spring Green Preset Color)		Specifies a color with RGB value (0,255,127) 
				if(str == "steelBlue")			{RGB = 0x4682B4; break;} // (Steel Blue Preset Color)		Specifies a color with RGB value (70,130,180) 
				break;
			case 't':
				if(str == "tan")				{RGB = 0xD2B48C; break;} // (Tan Preset Color)				Specifies a color with RGB value (210,180,140) 
				if(str == "teal")				{RGB = 0x008080; break;} // (Teal Preset Color)				Specifies a color with RGB value (0,128,128) 
				if(str == "thistle")			{RGB = 0xD8BFD8; break;} // (Thistle Preset Color)			Specifies a color with RGB value (216,191,216) 
				if(str == "tomato")				{RGB = 0xFF7347; break;} // (Tomato Preset Color)			Specifies a color with RGB value (255,99,71) 
				if(str == "turquoise")			{RGB = 0x40E0D0; break;} // (Turquoise Preset Color)		Specifies a color with RGB value (64,224,208) 
				break;
			case 'v':
				if(str == "violet")				{RGB = 0xEE82EE; break;} // (Violet Preset Color)			Specifies a color with RGB value (238,130,238) 
				break;
			case 'w':
				if(str == "wheat")				{RGB = 0xF5DEB3; break;} // (Wheat Preset Color)			Specifies a color with RGB value (245,222,179) 
				if(str == "white")				{RGB = 0xFFFFFF; break;} // (White Preset Color)			Specifies a color with RGB value (255,255,255) 
				if(str == "whiteSmoke")			{RGB = 0xF5F5F5; break;} // (White Smoke Preset Color)		Specifies a color with RGB value (245,245,245) 
				break;
			case 'y':
				if(str == "yellow")				{RGB = 0xFFFF00; break;} // (Yellow Preset Color)			Specifies a color with RGB value (255,255,0) 
				if(str == "yellowGreen")		{RGB = 0x9ACD32; break;} // (Yellow Green Preset Color)		Specifies a color with RGB value (154,205,50) 
				break;
			}
			/*blue	= static_cast<unsigned char>(RGB & 0xFF);
			green	= static_cast<unsigned char>((RGB & 0xFF00)>>8);
			red		= static_cast<unsigned char>((RGB & 0xFF0000)>>16);*/
			/*color.R = static_cast<unsigned char>(RGB);
			color.G = static_cast<unsigned char>(RGB>>8);
			color.B = static_cast<unsigned char>((RGB)>>16);
			color.A = 0;*/

			color.R = (BYTE)(RGB >>16);
			color.G = (BYTE)(RGB >> 8);
			color.B = (BYTE)(RGB);
			color.A = 0;		
		}
		return color;
	}


	BYTE getOpacityFromString(const CString opacityStr)
	{
		BYTE alpha;
		if (opacityStr.Find(_T("f")) != -1)
			alpha = (BYTE) (XmlUtils::GetDouble(opacityStr) / 65536 * 256);
		else
			alpha = (BYTE)XmlUtils::GetDouble(opacityStr) * 256;
		return alpha;
	}

	long getRealFromString( const CString& str )
	{
		long val = 0;

		if (str.Find(_T("f")) != -1)
			val = XmlUtils::GetInteger(str);
		else
			val = (long)(XmlUtils::GetDouble(str) * 65536);

		return val;
	}


	CColor_ getColor2FromString(const CString colorStr, CColor_ color1)
	{
		CColor_ color;
		if (colorStr.Find(_T("fill darken")) != -1)
		{
			int p = XmlUtils::GetInteger(colorStr.Mid(colorStr.Find(_T("(")) + 1, colorStr.Find(_T(")")) - colorStr.Find(_T("(")) - 1));
			color.R = color1.R * p / 255;
			color.G = color1.G * p / 255;
			color.B = color1.B * p / 255;
		}
		else if (colorStr.Find(_T("fill lighten")) != -1)
		{
			int p = XmlUtils::GetInteger(colorStr.Mid(colorStr.Find(_T("(")) + 1, colorStr.Find(_T(")")) - colorStr.Find(_T("(")) - 1));
			color.R = 255 - (255 - color1.R)* p / 255;
			color.G = 255 - (255 - color1.G)* p / 255;
			color.B = 255 - (255 - color1.B)* p / 255;
		}		
		else
		{
			color = getColorFromString(colorStr);
		}
		return color;
	}
};