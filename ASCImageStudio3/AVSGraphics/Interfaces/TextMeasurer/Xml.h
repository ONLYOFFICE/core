#pragma once

namespace ImageStudio {
	namespace Serialize {
		namespace Paint {
			namespace Common {
				inline static BSTR GetAttribute(XML::IXMLDOMNamedNodeMapPtr pAttributes, _bstr_t bsName)
				{
					IXMLDOMNodePtr tempNode = pAttributes->getNamedItem(bsName);

					if (NULL == tempNode)
						return NULL;

					BSTR bsValue;
					tempNode->get_text(&bsValue);
					return bsValue;
				}

				static void FromXmlString(CString& strText)
				{
					strText.Replace(_T("&apos;"),	_T("'"));
					strText.Replace(_T("&lt;"),		_T("<"));
					strText.Replace(_T("&gt;"),		_T(">"));
					strText.Replace(_T("&quot;"),	_T("\""));
					strText.Replace(_T("&amp;"),	_T("&"));
				}
				class CXmlAttributeReader
				{
				public:
					static float	FloatAttributeFromNode(XmlUtils::CXmlNode &oXmlNode, const CString &strNode, 
						const CString &strAttribute, const float fDefaultValue = 0.0f)
					{
						XmlUtils::CXmlNode oNode;
						if (oXmlNode.GetNode(strNode, oNode))
						{
							return oNode.ReadAttributeOrValueFloat(strAttribute, fDefaultValue);
						}
						else
							return fDefaultValue;
					}
					// ---
					static REAL	FloatPtAttributeFromXml(XmlUtils::CXmlNode &oXmlNode, const CString &strAttribute, 
						REAL fDefaultValue = 0)
					{
						CString strAttributeValue = oXmlNode.GetAttributeOrValue(strAttribute, _T("NaNpt"));

						if ((strAttributeValue == _T("NaNpt")) || (strAttributeValue == _T("inherit")) || (strAttributeValue == _T("auto")))
						{
							return fDefaultValue;
						}

						CString strMetric	= _T("");
						GetMetric(strAttributeValue, strMetric);

						if (strAttributeValue.GetLength() == 0)
						{
							return fDefaultValue;
						}
						else
						{
							return ConvertToPt(strAttributeValue, strMetric, fDefaultValue);
						}
					}
					static REAL	FloatPtFromBSTR(BSTR bsValue, REAL fDefaultValue = 0)
					{
						CString strAttributeValue = (CString)bsValue;

						if ((strAttributeValue == _T("inherit")) || (strAttributeValue == _T("auto")))
						{
							return fDefaultValue;
						}

						CString strMetric	= _T("");
						GetMetric(strAttributeValue, strMetric);

						if (strAttributeValue.GetLength() == 0)
						{
							return fDefaultValue;
						}
						else
						{
							return ConvertToPt(strAttributeValue, strMetric, fDefaultValue);
						}
					}
					static REAL FloatMmAttributeFromXml(XmlUtils::CXmlNode &oXmlNode, const CString &strAttribute, 
						REAL fDefaultValue = 0)
					{
						CString strAttributeValue = oXmlNode.GetAttributeOrValue(strAttribute, _T("NaNpt"));

						if ((strAttributeValue == _T("NaNpt")) || (strAttributeValue == _T("inherit")))
						{
							return fDefaultValue;
						}

						CString strMetric	= _T("");
						GetMetric(strAttributeValue, strMetric);

						if (strAttributeValue.GetLength() == 0)
						{
							return fDefaultValue;
						}
						else
						{
							return ConvertToMm(strAttributeValue, strMetric, fDefaultValue);
						}
					}
					// ---
					static REAL	FloatPtFromString(const CString &strValue)
					{
						CString strTmpValue	= strValue;
						CString strMetric = _T("");
						GetMetric(strTmpValue, strMetric);

						if (strTmpValue.GetLength() == 0)
						{
							return 0.0f;
						}
						else
						{
							return ConvertToPt(strTmpValue, strMetric);
						}
					}
					// ---
					static CString	TextFromNode(XmlUtils::CXmlNode &oXmlNode, const CString &strNode)
					{
						XmlUtils::CXmlNode oNode;
						if (oXmlNode.GetNode(strNode, oNode))
							return oNode.GetText();
						else
							return _T("");
					}
					// ---
					static LONG LongAttributeHexFromXml(XmlUtils::CXmlNode &oXmlNode, const CString &strAttributeName, const LONG &iDefaultValue)
					{
						CString strAttrubuteValue	= oXmlNode.GetAttributeOrValue(strAttributeName, _T("NaN"));

						if ((strAttrubuteValue == _T("NaN")) || (strAttrubuteValue.GetLength() < 2))
						{
							return iDefaultValue;
						}
						else
						{
							if (strAttrubuteValue[0] == _T('#'))
								strAttrubuteValue.Delete(0, 1);

							strAttrubuteValue = _T("0x") + strAttrubuteValue;
						}

						LONG iResultValue	= iDefaultValue;

#ifdef _UNICODE
						swscanf_s(strAttrubuteValue, L"%x", &iResultValue);
#else
						sscanf_s(strAttrubuteValue, "%x", &iResultValue);
#endif

						return iResultValue;
					}

					static LONG LongHexFromBSTR(BSTR bsValue, LONG iDefaultValue)
					{
						CString strAttrubuteValue = (CString)bsValue;

						if (strAttrubuteValue.GetLength() < 2)
						{
							return iDefaultValue;
						}
						else
						{
							if (strAttrubuteValue[0] == _T('#'))
								strAttrubuteValue.Delete(0, 1);

							strAttrubuteValue = _T("0x") + strAttrubuteValue;
						}

						LONG iResultValue	= iDefaultValue;

#ifdef _UNICODE
						swscanf_s(strAttrubuteValue, L"%x", &iResultValue);
#else
						sscanf_s(strAttrubuteValue, "%x", &iResultValue);
#endif

						return iResultValue;
					}

					// ---
					static REAL ConvertToPt(const CString &strAttributeValue, const CString &strMetric, const REAL &fDefaultValue = 0)
					{
						REAL fAttributeValue	= (REAL)XmlUtils::GetDouble(strAttributeValue);

						if (strMetric == _T("mm"))
						{
							fAttributeValue	*= 72.0f / 25.4f;
						}
						else if (strMetric == _T("cm"))
						{
							fAttributeValue	*= 72.0f / 2.54f;
						}
						else if (strMetric == _T("in"))
						{
							fAttributeValue *= 72.0f;
						}
						else if (strMetric == _T("%"))
						{
							fAttributeValue = -fAttributeValue / 100.0f;
						}

						return fAttributeValue;
					}
					static REAL	ConvertToMm(const CString &strAttributeValue, const CString &strMetic, const REAL &fDefaultValue = 0)
					{
						REAL fAttributeValue	= (REAL)XmlUtils::GetDouble(strAttributeValue);

						if (strMetic == _T("cm"))
						{
							fAttributeValue	*= 10.0f;
						}
						else if (strMetic == _T("pt"))
						{
							fAttributeValue	*= 25.4f / 72.0f;
						}
						else if (strMetic == _T("in"))
						{
							fAttributeValue *= 25.4f;
						}
						else if (strMetic == _T("%"))
						{
							fAttributeValue	= fDefaultValue * fAttributeValue / 100.0f;
						}

						return fAttributeValue;
					}
					// ---
					static void	GetMetric(CString &strAttributeValue, CString &strMetric)
					{
						int iLength			= strAttributeValue.GetLength();

						if (strAttributeValue[iLength - 1] == _T('%'))
						{
							strMetric	= strAttributeValue.Mid(iLength - 1);
							strAttributeValue.Delete(iLength - 1, 1);
						}
						else
						{
							strMetric	= strAttributeValue.Mid(iLength - 2);
							strAttributeValue.Delete(iLength - 2, 2);
						}
					}
					// ---
					static CString	GetFontWeight(const NSStructuresGDI::CFont &oFont)
					{
						if (oFont.Bold == TRUE)
							return _T("bold");
						else
							return _T("normal");
					}
					static void		SetFontWeight(NSStructuresGDI::CFont &oFont, const CString &strFontWeight)
					{
						if (strFontWeight == _T("bold"))
							oFont.Bold	= TRUE;
						else if (strFontWeight == _T("normal"))
							oFont.Bold	= FALSE;
					}
					static CString	GetFontStyle(const NSStructuresGDI::CFont &oFont)
					{
						if (oFont.Italic == TRUE)
							return _T("italic");
						else
							return _T("normal");
					}
					static void		SetFontStyle(NSStructuresGDI::CFont &oFont, const CString &strFontStyle)
					{
						if (strFontStyle == _T("italic"))
							oFont.Italic	= TRUE;
						else if (strFontStyle == _T("normal"))
							oFont.Italic	= FALSE;
					}
					static CString	GetFontUnderline(const NSStructuresGDI::CFont &oFont)
					{
						if (oFont.Underline == 0)
							return _T("");
						else if (oFont.Underline == 1)
							return _T("single");
						else if (oFont.Underline == 2)
							return _T("double");
						else if (oFont.Underline == 3)
							return _T("thick");
						else if (oFont.Underline == 4)
							return _T("dotted");
						else if (oFont.Underline == 5)
							return _T("dottedHeavy");
						else if (oFont.Underline == 6)
							return _T("dash");
						else if (oFont.Underline == 7)
							return _T("dashedHeavy");
						else if (oFont.Underline == 8)
							return _T("dashLong");
						else if (oFont.Underline == 9)
							return _T("dashLongHeavy");
						else if (oFont.Underline == 10)
							return _T("dotDash");
						else if (oFont.Underline == 11)
							return _T("dashDotHeavy");
						else if (oFont.Underline == 12)
							return _T("dotDotDash");
						else if (oFont.Underline == 13)
							return _T("dashDotDotHeavy");
						else if (oFont.Underline == 14)
							return _T("wave");
						else if (oFont.Underline == 15)
							return _T("wavyHeavy");
						else if (oFont.Underline == 16)
							return _T("wavyDouble");
						return _T("");
					}
					static void		SetFontUnderline(NSStructuresGDI::CFont &oFont, const CString &strUnderline)
					{
						if (strUnderline == _T(""))
							oFont.Underline = 0;
						else if (strUnderline == _T("single"))
							oFont.Underline = 1;
						else if (strUnderline == _T("double"))
							oFont.Underline = 2;
						else if (strUnderline == _T("thick"))
							oFont.Underline = 3;
						else if (strUnderline == _T("dotted"))
							oFont.Underline = 4;
						else if (strUnderline == _T("dottedHeavy"))
							oFont.Underline = 5;
						else if (strUnderline == _T("dash"))
							oFont.Underline = 6;
						else if (strUnderline == _T("dashedHeavy"))
							oFont.Underline = 7;
						else if (strUnderline == _T("dashLong"))
							oFont.Underline = 8;
						else if (strUnderline == _T("dashLongHeavy"))
							oFont.Underline = 9;
						else if (strUnderline == _T("dotDash"))
							oFont.Underline = 10;
						else if (strUnderline == _T("dashDotHeavy"))
							oFont.Underline = 11;
						else if (strUnderline == _T("dotDotDash"))
							oFont.Underline = 12;
						else if (strUnderline == _T("dashDotDotHeavy"))
							oFont.Underline = 13;
						else if (strUnderline == _T("wave"))
							oFont.Underline = 14;
						else if (strUnderline == _T("wavyHeavy"))
							oFont.Underline = 15;
						else if (strUnderline == _T("wavyDouble"))
							oFont.Underline = 16;
					}
					static CString	GetFontStrikeout(NSStructuresGDI::CFont &oFont, const CString &strStrikeout)
					{
						if (oFont.Strikeout == 0)
							return _T("");
						else if (oFont.Strikeout == 1)
							return _T("single");
						else if (oFont.Strikeout == 2)
							return _T("double");
						return _T("");
					}
					static void		SetFontStrikeout(NSStructuresGDI::CFont &oFont, const CString &strStrikeout)
					{
						if (strStrikeout == _T(""))
							oFont.Strikeout = 0;
						else if (strStrikeout == _T("single"))
							oFont.Strikeout = 1;
						else if (strStrikeout == _T("double"))
							oFont.Strikeout = 2;
					}
					static CString	GetLeaderTab(const byte &bLeaderTab)
					{
						if (bLeaderTab == 0)
							return _T("");
						else if (bLeaderTab == 1)
							return _T("dot");
						else if (bLeaderTab == 2)
							return _T("hyphen");
						else if (bLeaderTab == 3)
							return _T("underscore");
						return _T("");
					}
					static byte		SetLeaderTab(const CString &strLeaderTab)
					{
						if (strLeaderTab == _T("") || strLeaderTab == _T("none"))
							return 0;
						else if (strLeaderTab == _T("dot"))
							return 1;
						else if (strLeaderTab == _T("hyphen"))
							return 2;
						else if (strLeaderTab == _T("underscore"))
							return 3;
						return 0;
					}
					static CString	GetTypeTab(const byte &bTypeTab)
					{
						if (bTypeTab == 0)
							return _T("left");
						else if (bTypeTab == 1)
							return _T("right");
						else if (bTypeTab == 2)
							return _T("num");
						return _T("left");
					}
					static byte		SetTypeTab(const CString &strTypeTab)
					{
						if (strTypeTab == _T("left"))
							return 0;
						else if (strTypeTab == _T("right"))
							return 1;
						else if (strTypeTab == _T("num"))
							return 2;
						return 0;
					}
					// ---
					static void		SetPenStyle(NSStructuresGDI::CPen& oPen, const CString& strPenStyle)
					{
						if (strPenStyle == _T("solid"))
						{
							//oPen.SetStyle(0);							
						}
						else if (strPenStyle == _T("dashed"))
						{
							//oPen.SetStyle(1);
						}
						else if (strPenStyle == _T("dotted"))
						{
							//oPen.SetStyle(2);
						}
					}
					// ---
					static CString	GetBaselineShift(const WORD &nBaselineShift)
					{
						if (nBaselineShift == 1)
						{
							return _T("super");
						}
						else if (nBaselineShift == 2)
						{
							return _T("sub");
						}
						else
						{
							return _T("baseline");
						}
					}
					static void		SetBaselineShift(WORD &nBaselineShift, REAL &fBaselineShift, const CString &strBaselineShift)
					{
						if (strBaselineShift == _T("super"))
						{
							nBaselineShift	= 1;
							fBaselineShift	= 0.0f;
						}
						else if (strBaselineShift == _T("sub"))
						{
							nBaselineShift	= 2;
							fBaselineShift	= 0.0f;
						}
						else if (strBaselineShift == _T("baseline"))
						{
							nBaselineShift	= 0;
							fBaselineShift	= 0.0f;
						}
						else
						{
							nBaselineShift	= 0;
							fBaselineShift	= FloatPtFromString(strBaselineShift);
						}
					}
					// ---
					static CString	GetTextAlignment(const WORD &nTextAlignment)
					{
						if (nTextAlignment == 0)
						{
							return _T("left");
						}
						else if (nTextAlignment == 1)
						{
							return _T("center");
						}
						else if (nTextAlignment == 2)
						{
							return _T("right");
						}
						else
						{
							return _T("justify");
						}
					}
					static void		SetTextAlignment(WORD &nTextAlignment, const CString &strTextAlignment)
					{
						if (strTextAlignment == _T("center"))
						{
							nTextAlignment	= 1;
						}
						else if (strTextAlignment == _T("right"))
						{
							nTextAlignment	= 2;
						}
						else if (strTextAlignment == _T("justify"))
						{
							nTextAlignment	= 3;
						}
						else if (strTextAlignment == _T("left"))
						{
							nTextAlignment	= 0;
						}
					}
					// ---
					static CString	GetBreakType(const int& breakType)
					{
						if (breakType == 1)
							return _T("page");
						else if (breakType == 2)
							return _T("column");
						else
							return _T("");
					}
					static void		SetBreakType(int& breakType, const CString& breakTypeStr)
					{
						if (breakTypeStr == _T("page"))
							breakType = 1;
						else if (breakTypeStr == _T("column"))
							breakType = 2;
						else
							breakType = 0;
					}
					// ---
					static void		SetRelativeFrom(BYTE& nRelativeFrom, const CString& strRelativeFrom)
					{
						if (strRelativeFrom == _T("page"))
						{
							nRelativeFrom	= 0;
						}
						else if (strRelativeFrom == _T("margin"))
						{
							nRelativeFrom	= 1;
						}
						else
						{
							nRelativeFrom	= 2;
						}
					}
					// ---
					static void		SetAlternateType(BYTE& nType, const CString& strType)
					{
						if (strType == _T("topAndBottom"))
						{
							nType	= 1;
						}
						else if (strType == _T("square"))
						{
							nType	= 2;
						}
						else if (strType == _T("through"))
						{
							nType	= 2;
						}
						else if (strType == _T("tight"))
						{
							nType	= 3;
						}
						else if (strType == _T("none"))
						{
							nType	= 4;
						}
						else if (strType == _T("inline"))
						{
							nType	= 0;
						}
					}
					// ---
					static void		SetAlternatePosition(BYTE& nPosition, const CString& strPosition)
					{
						nPosition	= 0;
						if (strPosition == _T("center"))
						{
							nPosition	= 1;
						}
						else if (strPosition == _T("right"))
						{
							nPosition	= 2;
						}
					}
					// ---
					static void		SetKeepNext(BYTE& nKeepNext, const CString& strKeepNext)
					{
						nKeepNext	= 0;
						if (strKeepNext == _T("always"))
						{
							nKeepNext	= 1;
						}
					}
					static void		SetKeepTogether(BYTE& nKeepTogether, const CString& strKeepTogether)
					{
						nKeepTogether	= 0;
						if (strKeepTogether == _T("always"))
						{
							nKeepTogether	= 1;
						}
					}
					// ---
					static LONG	BGRToRGB(const LONG &iBGR)
					{
						LONG iRed	= (iBGR & 0x00FF0000) >> 16;
						LONG iGreen	= (iBGR & 0x0000FF00);
						LONG iBlue	= (iBGR & 0x000000FF) << 16;

						LONG iRGB	= iRed | iGreen | iBlue;

						if (iRGB != 0x00FFFFFF)
						{
							return iRGB;							
						}
						else
						{
							return 0;
						}
					}
				};
				class CXmlAttributeReader2
				{
				public:
					static float	TextToPt(XmlUtils::CXmlNode& oNode, const CString& strChildNode = _T(""), const float& fDefaultValue = 0)
					{
						CString strValue = _T("");
						if (strChildNode == _T(""))
						{
							strValue = oNode.GetText();
							return ToPt(strValue, fDefaultValue);
						}
						else
						{
							XmlUtils::CXmlNode oChildNode;
							if (oNode.GetNode(strChildNode, oChildNode))
								return TextToPt(oChildNode, _T(""), fDefaultValue);
						}

						return fDefaultValue;
					}
					static CString	AttrToStr(XmlUtils::CXmlNode& oNode, const CString& strChildNode, const CString& strAttrName)
					{
						if (strChildNode == _T(""))
							return oNode.GetAttribute(strAttrName);
						else
						{
							XmlUtils::CXmlNode oChildNode;
							if (oNode.GetNode(strChildNode, oChildNode))
								return oChildNode.GetAttribute(strAttrName);
						}
						return _T("");
					}
					static float	AttrToPt(XmlUtils::CXmlNode& oNode, const CString& strChildNode, const CString& strAttrName, const float& fDefaultValue = 0)
					{
						CString strValue = _T("");
						if (strChildNode == _T(""))
						{
							strValue = oNode.GetAttribute(strAttrName);
							return ToPt(strValue, fDefaultValue);
						}
						else
						{
							XmlUtils::CXmlNode oChildNode;
							if (oNode.GetNode(strChildNode, oChildNode))
								return AttrToPt(oChildNode, _T(""), strAttrName, fDefaultValue);
						}

						return fDefaultValue;
					}
				private:
					static float	ToPt(CString& strValue, const float& fDefaultValue = 0)
					{
						CString strMetric = Metric(strValue);
						if (strValue.GetLength() == 0)
							return fDefaultValue;

						float fValue = (float)XmlUtils::GetDouble(strValue);
						if (strMetric == _T("mm"))
							fValue	*= 72.0f / 25.4f;
						else if (strMetric == _T("cm"))
							fValue	*= 72.0f / 2.54f;
						else if (strMetric == _T("in"))
							fValue *= 72.0f;
						else if (strMetric == _T("%"))
							fValue = -fValue / 100.0f;
						else if (strMetric == _T(""))
							fValue /= 20.0f;

						return fValue;
					}
					static CString	Metric(CString& strValue)
					{
						CString strMetric;
						int iLength	= strValue.GetLength();
						if (isdigit(strValue[iLength - 1]))
							strMetric = _T("");
						else if (strValue[iLength - 1] == _T('%'))
							strMetric = strValue.Mid(iLength - 1);
						else
							strMetric = strValue.Mid(iLength - 2);
						int iMetricLength = strMetric.GetLength();
						strValue.Delete(iLength - iMetricLength, iMetricLength);

						return strMetric;
					}
				};
			}
		}
	}
}