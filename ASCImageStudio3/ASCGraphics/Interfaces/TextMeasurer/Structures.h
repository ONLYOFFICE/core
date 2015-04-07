#pragma once

#include "Xml.h"
#include "XmlConst.h"

namespace ImageStudio {
	namespace Serialize {
		namespace Paint {
			namespace Common {
				struct CFont : private CXmlAttributeReader
				{
				public:
					NSStructuresGDI::CFont m_oFont;

				public:
					CFont()
					{
						m_oFont.Name = _T("Times New Roman");

						m_oFont.Size = 10;

						m_oFont.Bold = 0;
						m_oFont.Italic = 0;
						m_oFont.Underline = 0;
						m_oFont.Strikeout = 0;
					}
				public:
					void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
					{
						CString strDefaultValue;
						CString strFamilyName;						
						strDefaultValue.Format(_T("%s"), m_oFont.Name);
						strFamilyName	= oXmlNode.GetAttributeOrValue(_T("font-name"), strDefaultValue);
						m_oFont.Name = strFamilyName;
						FontFamily oFontFamily(strFamilyName);

						strDefaultValue.Format(_T("%f"), m_oFont.Size);
						m_oFont.Size = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("font-size"), strDefaultValue));
						if (m_oFont.Size <= 0)
						{
							m_oFont.Size = 12;
						}

						strDefaultValue.Format(_T("%i"), m_oFont.Bold);
						m_oFont.Bold = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-bold"), strDefaultValue));
						if ((!oFontFamily.IsStyleAvailable(Gdiplus::FontStyleRegular)) && ((oFontFamily.IsStyleAvailable(Gdiplus::FontStyleItalic))))
						{
							m_oFont.Italic = true;
						}
						else
						{
							strDefaultValue.Format(_T("%i"), m_oFont.Italic);
							m_oFont.Italic = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-italic"), strDefaultValue));
						}
						strDefaultValue.Format(_T("%i"), m_oFont.Underline);
						m_oFont.Underline = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("font-underline"), strDefaultValue));
						strDefaultValue.Format(_T("%i"), m_oFont.Strikeout);
						m_oFont.Strikeout = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("strikeout"), strDefaultValue));
					}
					void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
					{
						CString strDefaultValue;
						strDefaultValue.Format(_T("%s"), m_oFont.Name);
						strDefaultValue	= oXmlNode.GetAttributeOrValue(_T("font-family"), strDefaultValue);
						if (strDefaultValue != _T(""))
							m_oFont.Name = strDefaultValue;

						strDefaultValue.Format(_T("%f"), m_oFont.Size);
						REAL fFontSize	= FloatPtAttributeFromXml(oXmlNode, _T("font-size"), (REAL)m_oFont.Size);
						if (fFontSize > 0)
							m_oFont.Size = fFontSize;
						else if (fFontSize != 0)
							m_oFont.Size = -fFontSize * m_oFont.Size;
						else
							m_oFont.Size = 12;

						strDefaultValue.Format(_T("%s"), GetFontWeight(m_oFont));
						SetFontWeight(m_oFont, oXmlNode.GetAttributeOrValue(_T("font-weight"), strDefaultValue));
						strDefaultValue.Format(_T("%s"), GetFontStyle(m_oFont));
						SetFontStyle(m_oFont, oXmlNode.GetAttributeOrValue(_T("font-style"), strDefaultValue));
						ReadUnderline(oXmlNode);
						ReadStrikeout(oXmlNode);

						m_oFont.CharSpace = XmlUtils::GetDouble(oXmlNode.GetAttribute(_T("letter-spacing"), _T("0"))) * 25.4f / 72.0f;
					}

					void	FromXmlAttributes(XML::IXMLDOMNamedNodeMapPtr pAttributes)
					{
						BSTR bsValue;

						bsValue = GetAttribute(pAttributes, g_cpszXML_Font_Family);
						if (NULL != bsValue)
						{
							m_oFont.Name = bsValue;
							SysFreeString(bsValue);
						}
						bsValue = GetAttribute(pAttributes, g_cpszXML_Font_Size);
						if (NULL != bsValue)
						{
							REAL fFontSize	= FloatPtFromBSTR(bsValue, (REAL)m_oFont.Size);
							if (fFontSize > 0)
								m_oFont.Size = fFontSize;
							else if (fFontSize != 0)
								m_oFont.Size = -fFontSize * m_oFont.Size;
							else
								m_oFont.Size = 12;

							SysFreeString(bsValue);
						}
						bsValue = GetAttribute(pAttributes, g_cpszXML_Font_Weight);
						if (NULL != bsValue)
						{
							CString strWeight = (CString)bsValue;
							SetFontWeight(m_oFont, strWeight);
							SysFreeString(bsValue);
						}
						bsValue = GetAttribute(pAttributes, g_cpszXML_Font_Style);
						if (NULL != bsValue)
						{
							CString strStyle = (CString)bsValue;
							SetFontStyle(m_oFont, strStyle);
							SysFreeString(bsValue);
						}
						bsValue = GetAttribute(pAttributes, g_cpszXML_Font_Underline);
						if (NULL != bsValue)
						{
							CString strUnderline = (CString)bsValue;
							SetFontUnderline(m_oFont, strUnderline);
							SysFreeString(bsValue);
						}
						bsValue = GetAttribute(pAttributes, g_cpszXML_Font_Strike);
						if (NULL != bsValue)
						{
							CString strStrike = (CString)bsValue;
							SetFontStrikeout(m_oFont, strStrike);
							SysFreeString(bsValue);
						}
						bsValue = GetAttribute(pAttributes, g_cpszXML_Font_Letter_Spacing);
						if (NULL != bsValue)
						{
							m_oFont.CharSpace = (double)FloatPtFromBSTR(bsValue, (float)m_oFont.CharSpace);
							m_oFont.CharSpace *= 25.4f / 72.0f;
							SysFreeString(bsValue);
						}
					}

					void	FromFont(const NSStructures::CFont& font)
					{
						m_oFont.Name      = font.Name;
						m_oFont.Path	  = font.Path;
						m_oFont.Size      = font.Size;
						m_oFont.Bold      = font.Bold;
						m_oFont.Italic    = font.Italic;
						m_oFont.Underline = font.Underline;
						m_oFont.Strikeout = font.Strikeout;
						m_oFont.StringGID = font.StringGID;
					}
				public:
					Gdiplus::Font*	GetGdiFont()
					{
						return m_oFont.GetFont(UnitPoint);
					}
					void			SetToRenderer(IAVSRenderer *pRenderer)
					{
						m_oFont.SetToRenderer(pRenderer);
					}

				public:
					const CString&	Name() const
					{
						return m_oFont.Name;
					}
					void SetName(const CString& fontFamily)
					{
						m_oFont.Name = fontFamily;
					}
					float	Size() const 
					{
						return (float)m_oFont.Size;
					}
					LONG	Style() const 
					{
						return m_oFont.GetStyle();
					}

				private:
					void	ReadUnderline(XmlUtils::CXmlNode &oFontNode)
					{
						CString strValue = oFontNode.GetAttributeOrValue(_T("underline"));
						if (strValue != _T(""))
							SetFontUnderline(m_oFont, strValue);
					}
					void	ReadStrikeout(XmlUtils::CXmlNode &oFontNode)
					{
						CString strValue = oFontNode.GetAttributeOrValue(_T("strike"));
						if (strValue != _T(""))
							SetFontStrikeout(m_oFont, strValue);
					}
					void	ToXmlWriter(XmlUtils::CXmlWriter &oXmlWriter)
					{
						m_oFont.ToXmlAttributes(oXmlWriter);
					}
				};
				struct CBrush : private CXmlAttributeReader
				{
				public:
					NSStructuresGDI::CBrush m_oBrush;						

				public:
					// ---
					CBrush()
					{
						m_oBrush.Type = 0;

						m_oBrush.Color1 = 0;
						m_oBrush.Color2 = 0;

						m_oBrush.Alpha1 = 255;
						m_oBrush.Alpha2 = 255;

						m_oBrush.TextureAlpha = 255;
					}
					// ---
					void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
					{
						CString strDefaultValue	= _T("");
						strDefaultValue.Format(_T("%i"), m_oBrush.Type);
						m_oBrush.Type = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-type"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oBrush.Color1);
						m_oBrush.Color1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-color1"), strDefaultValue));
						strDefaultValue.Format(_T("%i"), m_oBrush.Color2);
						m_oBrush.Color2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-color2"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oBrush.Alpha1);
						m_oBrush.Alpha1 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-alpha1"), strDefaultValue));
						strDefaultValue.Format(_T("%i"), m_oBrush.Alpha2);
						m_oBrush.Alpha2 = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-alpha2"), strDefaultValue));

						strDefaultValue.Format(_T("%s"), m_oBrush.TexturePath);
						m_oBrush.TexturePath = oXmlNode.GetAttributeOrValue(_T("brush-texturepath"), strDefaultValue);

						strDefaultValue.Format(_T("%i"), m_oBrush.TextureAlpha);
						m_oBrush.TextureAlpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-texturealpha"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oBrush.TextureMode);
						m_oBrush.TextureMode = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("brush-texturemode"), strDefaultValue));

						/*strDefaultValue.Format(_T("%i"), m_oBrush.GetRectable());
						m_oBrush.SetRectable(XmlUtils::GetBoolean(oXmlNode.GetAttributeOrValue("brush-rectable"), strDefaultValue)));
						Gdiplus::RectF oRectF = m_oBrush.GetRect();
						strDefaultValue.Format(_T("%f"), oRectF.X);
						oRectF.X = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue("brush-rect-left"), strDefaultValue));
						strDefaultValue.Format(_T("%f"),oRectF.Y);
						oRectF.Y = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue("brush-rect-top"), strDefaultValue));
						strDefaultValue.Format(_T("%f"), oRectF.Width);
						oRectF.Width = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue("brush-rect-width"), strDefaultValue));
						strDefaultValue.Format(_T("%f"), oRectF.Height);
						oRectF.Height = (REAL)XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue("brush-rect-height"), strDefaultValue));
						m_oBrush.SetRect(oRectF);*/
					}
					void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
					{
#ifdef DD_BRUSH_XML
						CString strXml	= oXmlNode.GetXml();
#endif

						LONG iColor1	= LongAttributeHexFromXml(oXmlNode, _T("color"), m_oBrush.Color1);
						if (iColor1 != 0xFFFFFF)
							m_oBrush.Color1 = BGRToRGB(iColor1);
						else
							m_oBrush.Color1 = 0xFFFFFF;
					}	

					void	FromXmlAttributes(XML::IXMLDOMNamedNodeMapPtr pAttributes)
					{
						BSTR bsValue;

						bsValue = GetAttribute(pAttributes, g_cpszXML_Brush_Color);
						if (NULL != bsValue)
						{
							LONG iColor1	= LongHexFromBSTR(bsValue, m_oBrush.Color1);
							if (iColor1 != 0xFFFFFF)
								m_oBrush.Color1 = BGRToRGB(iColor1);
							else
								m_oBrush.Color1 = 0xFFFFFF;

							SysFreeString(bsValue);
						}
					}

					void	FromBrush(const NSStructures::CBrush& brush)
					{
						m_oBrush.Type    = brush.Type;

						m_oBrush.Color1  = brush.Color1;
						m_oBrush.Alpha1  = brush.Alpha1;
						m_oBrush.Color2  = brush.Color2;
						m_oBrush.Alpha2  = brush.Alpha2;

						m_oBrush.TexturePath  = brush.TexturePath;
						m_oBrush.TextureAlpha = brush.TextureAlpha;
						m_oBrush.TextureMode  = brush.TextureMode;

						m_oBrush.Rectable = brush.Rectable;
						m_oBrush.Rect     = brush.Rect;

						m_oBrush.LinearAngle = brush.LinearAngle;
					}
					// ---
					void	ToXmlWriter(XmlUtils::CXmlWriter &oXmlWriter)
					{
						m_oBrush.ToXmlAttributes(oXmlWriter);
					}
					// ---
					Gdiplus::Brush*	GetGdiBrush()
					{
						return m_oBrush.GetBrush();
					}
					void	SetToRenderer(IAVSRenderer *pRenderer)
					{
						m_oBrush.SetToRenderer(pRenderer);
						
					}
					void SetToRenderer(IAVSRenderer* renderer, const float& transparency)
					{
						m_oBrush.SetToRenderer(renderer, transparency);
					}
					// ---
				};

				struct CPen : private CXmlAttributeReader
				{
				public:
					NSStructuresGDI::CPen m_oPen;

				public:
					CPen()
					{
						m_oPen.Color = 0;

						m_oPen.Alpha = 255;

						m_oPen.Size = 0.3528f;
					}
				public:
					void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
					{
						CString strDefaultValue;

						strDefaultValue.Format(_T("%i"), m_oPen.Color);
						m_oPen.Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-color"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oPen.Alpha);
						m_oPen.Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-alpha"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oPen.Size);
						m_oPen.Size = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("pen-size"), strDefaultValue))/* * 0.3528f*/;

						strDefaultValue.Format(_T("%i"), m_oPen.Align);
						m_oPen.Align = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-alignment"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oPen.DashStyle);
						m_oPen.DashStyle = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-style"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oPen.LineJoin);
						m_oPen.LineJoin = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-line-join"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oPen.LineStartCap);
						m_oPen.LineStartCap = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-line-start-cap"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oPen.LineEndCap);
						m_oPen.LineEndCap = (byte)XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-line-end-cap"), strDefaultValue));

						int nCount = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("pen-dash-pattern-count"), _T("0")));
						if (nCount != 0)
						{
							XmlUtils::CXmlNode oPatternNode;
							if (oXmlNode.GetNode(_T("dash-pattern"), oPatternNode))
							{
								XmlUtils::CXmlNodes oDashNodes;
								if (oPatternNode.GetNodes(_T("dash"), oDashNodes))
								{
									nCount = oDashNodes.GetCount();
									if (nCount != 0)
									{
										double* pDashPattern = new double[nCount];
										XmlUtils::CXmlNode oDashNode;
										for (int i = 0; i < nCount; ++i)
										{
											oDashNodes.GetAt(i, oDashNode);
											pDashPattern[i] = XmlUtils::GetDouble(oDashNode.GetText());
										}

										m_oPen.SetDashPattern(pDashPattern, nCount);
										RELEASEARRAYOBJECTS(pDashPattern);
									}
								}
							}
						}

						strDefaultValue.Format(_T("%i"), m_oPen.DashOffset);
						m_oPen.DashOffset = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("pen-dash-offset"), strDefaultValue));

						/*strDefaultValue.Format(_T("%i"), m_oPen.GetStyle());
						strDefaultValue	= oXmlNode.GetAttributeOrValue(_T("pen-style"), strDefaultValue);
						SetPenStyle(m_oPen, strDefaultValue);*/
					}
				public:
					Gdiplus::Pen*	GetGdiFont()
					{
						return m_oPen.GetPen();
					}
					void			SetToRenderer(IAVSRenderer *pRenderer)
					{
						m_oPen.SetToRenderer(pRenderer);
						/*if (m_oPen.Color < 0)
						{
						m_oPen.Color = 0;
						}
						XmlUtils::CXmlWriter oXmlWriter;
						ToXmlWriter(oXmlWriter);

						CString strXmlPen = _T("<pen");
						strXmlPen += oXmlWriter.GetXmlString();
						strXmlPen += _T("/>");

						BSTR bstrXmlPen = strXmlPen.AllocSysString();

						pRenderer->SetPen(bstrXmlPen);

						SysFreeString(bstrXmlPen);*/
					}
				private:
					void	ToXmlWriter(XmlUtils::CXmlWriter &oXmlWriter)
					{
						m_oPen.ToXmlAttributes(oXmlWriter);
					}
				};

				struct CShadow : private CXmlAttributeReader
				{
				public:
					NSStructuresGDI::CShadow m_oShadow;

				public:
					// ---
					CShadow()
					{
						m_oShadow.Visible = false;

						m_oShadow.Color = 0xFFFFFE;

						m_oShadow.Alpha = 0;

						m_oShadow.DistanceX = 2;
						m_oShadow.DistanceY = 2;

						m_oShadow.BlurSize = 0;
					}
					// ---
					void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
					{
						CString strDefaultValue = _T("");

						strDefaultValue.Format(_T("%i"), m_oShadow.Visible);
						m_oShadow.Visible = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("shadow-visible"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oShadow.Color);
						m_oShadow.Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("shadow-color"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oShadow.Alpha);
						m_oShadow.Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("shadow-alpha"), strDefaultValue));

						strDefaultValue.Format(_T("%f"), m_oShadow.DistanceX);
						m_oShadow.DistanceX = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("shadow-distancex"), strDefaultValue));
						strDefaultValue.Format(_T("%f"), m_oShadow.DistanceY);
						m_oShadow.DistanceY = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("shadow-distancey"), strDefaultValue));

						strDefaultValue.Format(_T("%f"), m_oShadow.BlurSize);
						m_oShadow.BlurSize = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("shadow-blursize"), strDefaultValue));
					}
					void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
					{
#ifdef DD_BRUSH_XML
						CString strXml	= oXmlNode.GetXml();
#endif						
					}

					void FromXmlAttributes(XML::IXMLDOMNamedNodeMapPtr pAttributes)
					{
						//  ,   FromXmlNode2
					}


					// ---
					void	ToXmlWriter(XmlUtils::CXmlWriter &oXmlWriter)
					{
						m_oShadow.ToXmlAttributes(oXmlWriter);
					}
					// ---
					void	SetToRenderer(IAVSRenderer *pRenderer)
					{
						pRenderer->put_ShadowVisible(m_oShadow.Visible);
						if (m_oShadow.Visible)
						{
							XmlUtils::CXmlWriter oXmlWriter;
							ToXmlWriter(oXmlWriter);

							CString strXmlShadow = _T("<shadow");
							strXmlShadow += oXmlWriter.GetXmlString();
							strXmlShadow += _T("/>");

							BSTR bstrXmlShadow = strXmlShadow.AllocSysString();

							pRenderer->SetShadow(bstrXmlShadow);

							SysFreeString(bstrXmlShadow);
						}
					}
				};

				struct CBackground : private CXmlAttributeReader
				{
				public:
					NSStructuresGDI::CBackground m_oBackground;

				public:
					// ---
					CBackground()
					{
						m_oBackground.Color = 0;
						m_oBackground.Alpha = 0;
					}
					// ---
					void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
					{						
					}
					void	FromXmlNode2(XmlUtils::CXmlNode &oXmlNode)
					{
#ifdef DD_BRUSH_XML
						CString strXml	= oXmlNode.GetXml();
#endif

						LONG iColor1	= LongAttributeHexFromXml(oXmlNode, _T("background-color"), m_oBackground.Color);						

						if (oXmlNode.GetAttribute(_T("background-color")) != _T(""))
						{
							m_oBackground.Alpha = 255;
							m_oBackground.Color = BGRToRGB(iColor1);
						}
						else
						{
							m_oBackground.Color = iColor1;
						}
					}
					void FromXmlAttributes(XML::IXMLDOMNamedNodeMapPtr pAttributes)
					{
						BSTR bsValue;

						bsValue = GetAttribute(pAttributes, g_cpszXML_Background_Color);
						if (NULL != bsValue)
						{
							LONG iColor1	= LongHexFromBSTR(bsValue, m_oBackground.Color);

							m_oBackground.Alpha = 255;
							if (iColor1 != 0xFFFFFF)
								m_oBackground.Color = BGRToRGB(iColor1);
							else
								m_oBackground.Color = 0xFFFFFF;


							SysFreeString(bsValue);
						}
						//else
						//{
						//	m_oBackground = m_oBackground;
						//}
					}

					// ---
					void	ToXmlWriter(XmlUtils::CXmlWriter &oXmlWriter)
					{
						m_oBackground.ToXmlAttributes(oXmlWriter);
					}
					// ---
					void	SetToRenderer(IAVSRenderer *pRenderer)
					{
						XmlUtils::CXmlWriter oXmlWriter;
						ToXmlWriter(oXmlWriter);

						CString strXmlBackground = _T("<background");
						strXmlBackground += oXmlWriter.GetXmlString();
						strXmlBackground += _T("/>");

						/*BSTR bstrXmlBackground = strXmlBackground.AllocSysString();

						pRenderer->SetBackground(bstrXmlBackground);

						SysFreeString(bstrXmlBackground);*/
					}
				};

				struct CEdge
				{
				private:
					NSStructuresGDI::CEdgeText m_oEdge;

				public:
					// ---
					CEdge()
					{
						m_oEdge.Visible = false;
						m_oEdge.Color = 0;
						m_oEdge.Alpha = 0;
						m_oEdge.Dist = 0;
					}
					// ---
					void	FromXmlNode(XmlUtils::CXmlNode &oXmlNode)
					{
						CString strDefaultValue;

						strDefaultValue.Format(_T("%i"), m_oEdge.Visible);
						m_oEdge.Visible = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-visible"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oEdge.Alpha);
						m_oEdge.Alpha = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("shadow-alpha"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oEdge.Color);
						m_oEdge.Color = XmlUtils::GetInteger(oXmlNode.GetAttributeOrValue(_T("edge-color"), strDefaultValue));

						strDefaultValue.Format(_T("%i"), m_oEdge.Dist);
						m_oEdge.Dist = XmlUtils::GetDouble(oXmlNode.GetAttributeOrValue(_T("edge-dist"), strDefaultValue));
					}

					void FromXmlAttributes(XML::IXMLDOMNamedNodeMapPtr pAttributes)
					{
						//   
					}
					// ---
					void	ToXmlWriter(XmlUtils::CXmlWriter &oXmlWriter)
					{
						m_oEdge.ToXmlAttributes(oXmlWriter);
					}
					// ---
					void	SetToRenderer(IAVSRenderer *pRenderer)
					{
						pRenderer->put_EdgeVisible(m_oEdge.Visible);

						if (0 != m_oEdge.Visible)
						{
							XmlUtils::CXmlWriter oXmlWriter;
							ToXmlWriter(oXmlWriter);

							CString strXmlEdge = _T("<edge");
							strXmlEdge += oXmlWriter.GetXmlString();
							strXmlEdge += _T("/>");

							BSTR bstrXmlEdge = strXmlEdge.AllocSysString();

							pRenderer->SetEdgeText(bstrXmlEdge);

							SysFreeString(bstrXmlEdge);
						}
					}
				};
			}
		}
	}
}
