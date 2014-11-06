//#include "stdafx.h"
#include "Elements.h"

namespace NSPresentationEditor
{
	CString NSPresentationEditor::CLayout::ToXmlEditor(NSPresentationEditor::CTheme* pTheme, const NSPresentationEditor::CMetricInfo& oInfo)
	{
		NSPresentationEditor::CStringWriter oWriter;

		CString strStart = _T("");
		strStart.Format(_T("<Layout usethemeobjects=\"%d\" type=\"%ls\">"), m_bShowMasterShapes ? 1 : 0, m_strLayoutType);

		oWriter.WriteString(strStart);

		if (!m_bUseThemeColorScheme)
		{
			// colors ----
			oWriter.WriteString(_T("<Colors>"));
			
			size_t nCountColors = m_arColorScheme.size();
			for (size_t i = 0; i < nCountColors; ++i)
			{
				CString strFormat = _T("");
				LONG lColorValue = m_arColorScheme[i].GetLONG_RGB();
				strFormat.Format(_T(".color%u { color: #%06x; } .stroke%u { stroke:#%06x; } .fill%u { fill:#%06x; }\n"), 
					i + 1, lColorValue, i + 1, lColorValue, i + 1, lColorValue);
				oWriter.WriteString(strFormat);
			}

			oWriter.WriteString(_T("</Colors>"));
			// -----------
		}

		if (m_bIsBackground)
		{
			// background
			#ifdef PPT_DEF
			CShapeElement oElem(NSPresentationEditor::NSBaseShape::ppt, PPTShapes::sptCRect);
			#else
			#ifdef ODP_DEF
			CShapeElement oElem(NSPresentationEditor::NSBaseShape::odp, OdpShapes::sptCRect);
			oElem.m_oShape.m_dWidthLogic	= ((COdpShape*)oElem.m_oShape.m_pShape)->FManager.GetValue(_T("width"));
			oElem.m_oShape.m_dHeightLogic	= ((COdpShape*)oElem.m_oShape.m_pShape)->FManager.GetValue(_T("height"));
			#else
			CShapeElement oElem(NSPresentationEditor::NSBaseShape::pptx, OOXMLShapes::sptCRect);
			#endif
			#endif

			oElem.m_oMetric = oInfo;
			oElem.m_rcBoundsOriginal.left	= 0.0;
			oElem.m_rcBoundsOriginal.right	= (double)oInfo.m_lUnitsHor;
			oElem.m_rcBoundsOriginal.top	= 0.0;
			oElem.m_rcBoundsOriginal.bottom	= (double)oInfo.m_lUnitsVer;

			double dScaleX = (double)oInfo.m_lMillimetresHor / oInfo.m_lUnitsHor;
			double dScaleY = (double)oInfo.m_lMillimetresVer / oInfo.m_lUnitsVer;

			oElem.NormalizeCoords(dScaleX, dScaleY);
			oElem.m_oShape.m_oBrush = m_oBackground;

			oElem.m_bIsBackground = true;
			oElem.m_bIsChangeable = false;

			oWriter.WriteString(_T("<Background>"));
			oElem.SetupProperties(NULL, pTheme, this);
			oWriter.WriteString(oElem.ToXmlEditor());
			oWriter.WriteString(_T("</Background>"));
		}
		// -----------
		// elements (no placeholders)
		oWriter.WriteString(_T("<Elements>"));

		size_t nCountElems = m_arElements.size();
		for (size_t i = 0; i < nCountElems; ++i)
		{
			if (-1 == m_arElements[i]->m_lPlaceholderType)
			{
				m_arElements[i]->m_bIsBackground = false;
				m_arElements[i]->m_bIsChangeable = false;

				oWriter.WriteString(m_arElements[i]->ToXmlEditor());
			}
		}

		oWriter.WriteString(_T("</Elements>"));
		// ----------------
		// placeholders
		oWriter.WriteString(_T("<Placeholders>"));

		CString strStylesPh = _T("");
		CString strFontRefs = _T("");

		for (size_t i = 0; i < nCountElems; ++i)
		{
			IElement* pElement = m_arElements[i]; 
			if (-1 != pElement->m_lPlaceholderType)
			{
				LONG lPercentL = (LONG)(pElement->m_rcBounds.left * 100 / m_lWidth);	
				LONG lPercentT = (LONG)(pElement->m_rcBounds.top * 100 / m_lHeight);
				LONG lPercentW = (LONG)(pElement->m_rcBounds.GetWidth() * 100 / m_lWidth);
				LONG lPercentH = (LONG)(pElement->m_rcBounds.GetHeight() * 100 / m_lHeight);

				CString strPlaceholder = _T("");
				strPlaceholder.Format(_T("<Placeholder id=\"%d\" type=\"%d\" left=\"%d\" top=\"%d\" width=\"%d\" height=\"%d\" angle=\"%lf\" flipx=\"%d\" flipy=\"%d\">"), 
					m_arElements[i]->m_lPlaceholderID, m_arElements[i]->m_lPlaceholderType, lPercentL, lPercentT, lPercentW, lPercentH,
					m_arElements[i]->m_dRotate, m_arElements[i]->m_bFlipH ? 1 : 0, m_arElements[i]->m_bFlipV ? 1 : 0);
				oWriter.WriteString(strPlaceholder);
				oWriter.WriteString(pElement->ToXmlEditor());

				if (etShape == pElement->m_etType)
				{
					CShapeElement* pTextElement = dynamic_cast<CShapeElement*>(pElement);
					if (NULL != pTextElement)
					{
						LONG lCountThisType = this->GetCountPlaceholderWithType(pTextElement->m_lPlaceholderType);
						LONG lID = -1;
						if (1 < lCountThisType)
							lID = pTextElement->m_lPlaceholderID;

						strStylesPh += pTextElement->m_oShape.m_oText.m_oStyles.ToXmlEditor(oInfo, true, pTextElement->m_lPlaceholderType, lID);

						LONG lFontRef = pTextElement->m_oShape.m_oText.m_lFontRef;
						if ((0 <= lFontRef) && (lFontRef < (LONG)pTheme->m_arFonts.size()))
						{
							CString strRef = _T("");
							strRef.Format(_T("l_font%d { font-index:%d;font-family:%ls; }\n"), 
								pTextElement->m_lPlaceholderType, lFontRef, pTheme->m_arFonts[lFontRef].Name);
							strFontRefs += strRef;
						}
					}
				}
				
				CString strEndPlaceHolder = _T("</Placeholder>");
				oWriter.WriteString(strEndPlaceHolder);
			}
		}

		oWriter.WriteString(_T("</Placeholders>"));

		oWriter.WriteString(_T("<Styles>") + strStylesPh + _T("</Styles>"));
		oWriter.WriteString(_T("<StylesFontRef>") + strFontRefs + _T("</StylesFontRef>"));

		oWriter.WriteString(_T("</Layout>"));

		return oWriter.GetData();
	}

	void NSPresentationEditor::CLayout::ReadFromXml(XmlUtils::CXmlNode& oNode)
	{
#ifdef _PRESENTATION_WRITER_
		m_strLayoutType		= oNode.GetAttribute(_T("type"));
		m_bShowMasterShapes = (oNode.ReadAttributeInt(_T("usethemeobjects")) == 1) ? true : false; 

		m_bUseThemeColorScheme = true;
		//colors
		m_arColorScheme.clear();
		XmlUtils::CXmlNode oNodeColors;
		if (oNode.GetNode(_T("Colors"), oNodeColors))
		{
			m_bUseThemeColorScheme = false;

			CStylesCSS oStyles;
			oStyles.LoadStyles(oNodeColors.GetText());

			size_t nCount = oStyles.m_arStyles.size();
			LONG lColor = 0;
			for (size_t i = 0; i < nCount; i += 3)
			{
				CColor elem;
				m_arColorScheme.push_back(elem);
				oStyles.m_arStyles[i].LoadColor(m_arColorScheme[lColor]);
				++lColor;
			}
		}

		m_bIsBackground = false;
		// background
		XmlUtils::CXmlNode oNodeBackround;
		if (oNode.GetNode(_T("Background"), oNodeBackround))
		{
			m_bIsBackground = true;
			XmlUtils::CXmlNode oNodeMem;
			if (oNodeBackround.GetNode(_T("shape"), oNodeMem))
			{
				CShapeElement oElem;
				oElem.m_oMetric = m_oInfo;
				oElem.LoadFromXmlNode2(oNodeMem);

				m_oBackground = oElem.m_oShape.m_oBrush;
			}				
		}

		// Elements
		XmlUtils::CXmlNode oNodeElements;
		if (oNode.GetNode(_T("Elements"), oNodeElements))
		{
			XmlUtils::CXmlNodes oNodes;
			if (oNodeElements.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNodeMem;
					oNodes.GetAt(i, oNodeMem);

					CShapeElement* pShapeEl = new CShapeElement();
					pShapeEl->m_oMetric = m_oInfo;
					pShapeEl->LoadFromXmlNode2(oNodeMem);
					m_arElements.push_back(pShapeEl);
				}
			}
		}

		// Placeholders
		XmlUtils::CXmlNode oNodePlaceholders;
		if (oNode.GetNode(_T("Placeholders"), oNodePlaceholders))
		{
			XmlUtils::CXmlNodes oNodes;
			if (oNodePlaceholders.GetNodes(_T("*"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNodeMem;
					oNodes.GetAt(i, oNodeMem);

					CShapeElement* pShapeEl = new CShapeElement();
					pShapeEl->m_oMetric = m_oInfo;

					CDoubleRect oRectBounds;
					oRectBounds.left		= oNodeMem.ReadAttributeDouble(_T("left")) * m_oInfo.m_lMillimetresHor / 100.0;
					oRectBounds.top			= oNodeMem.ReadAttributeDouble(_T("top")) * m_oInfo.m_lMillimetresVer / 100.0;
					oRectBounds.right		= oRectBounds.left + oNodeMem.ReadAttributeDouble(_T("width")) * m_oInfo.m_lMillimetresHor / 100.0;
					oRectBounds.bottom		= oRectBounds.top  + oNodeMem.ReadAttributeDouble(_T("height")) * m_oInfo.m_lMillimetresVer / 100.0;

					pShapeEl->LoadFromXmlNode(oNodeMem, &oRectBounds);
					pShapeEl->m_oShape.m_rcBounds = pShapeEl->m_rcBounds;

					pShapeEl->m_lPlaceholderID		= oNodeMem.ReadAttributeInt(_T("id"), -1);
					pShapeEl->m_lPlaceholderType	= oNodeMem.ReadAttributeInt(_T("type"), -1);

					m_arElements.push_back(pShapeEl);
				}
			}
		}

		size_t nCountElements = m_arElements.size();

		XmlUtils::CXmlNode oNodeStyles;
		if (oNode.GetNode(_T("Styles"), oNodeStyles))
		{
			XmlUtils::CXmlNodes oStyles;
			if (oNodeStyles.GetNodes(_T("Style"), oStyles))
			{
				int nCount = oStyles.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNodeS;
					oStyles.GetAt(i, oNodeS);

					LONG lPhType = oNodeS.ReadAttributeInt(_T("type"));

					CShapeElement* pElement = NULL;
					// нужно найти плэйсхолдер
					for (size_t nIndex = 0; nIndex < nCountElements; ++nIndex)
					{
						if (m_arElements[nIndex]->m_lPlaceholderType == lPhType)
						{
							if (m_arElements[nIndex]->m_etType == etShape)
							{
								pElement = dynamic_cast<CShapeElement*>(m_arElements[nIndex]);
								break;
							}
						}
					}
					
					if (NULL != pElement)
					{
						CStylesCSS oCSS;
						oCSS.LoadStyles(oNodeS.GetText());

						size_t nCountS = oCSS.m_arStyles.size();
						for (size_t nIndexStyle = 0; nIndexStyle < nCountS; ++nIndexStyle)
						{
							LONG _lPhType		= 0;
							LONG _lLevel		= 0;
							LONG _lTypeStyle	= -1;
							CLayout::CheckPlaceholderStyle(oCSS.m_arStyles[nIndexStyle].m_strClassName, _lPhType, _lLevel, _lTypeStyle);

							if (-1 != _lTypeStyle)
							{
								if (!pElement->m_oShape.m_oText.m_oStyles.m_pLevels[_lLevel].is_init())
									pElement->m_oShape.m_oText.m_oStyles.m_pLevels[_lLevel] = new NSPresentationEditor::CTextStyleLevel();
							}

							if (0 == _lTypeStyle)
							{
								oCSS.m_arStyles[nIndexStyle].LoadStylePF(pElement->m_oShape.m_oText.m_oStyles.m_pLevels[_lLevel]->m_oPFRun, m_oInfo);
							}
							else if (1 == _lTypeStyle)
							{
								oCSS.m_arStyles[nIndexStyle].LoadStyleCF(pElement->m_oShape.m_oText.m_oStyles.m_pLevels[_lLevel]->m_oCFRun, m_oInfo);
							}
						}
					}					
				}
			}
		}

		XmlUtils::CXmlNode oNodeFontRefs;
		if (oNode.GetNode(_T("StylesFontRef"), oNodeFontRefs))
		{
			CStylesCSS oCSS;
			oCSS.LoadStyles(oNodeFontRefs.GetText());

			size_t nCountS = oCSS.m_arStyles.size();
			for (size_t i = 0; i < nCountS; ++i)
			{
				oCSS.m_arStyles[i].m_strClassName.Delete(0, 6);
				LONG lPhType	= XmlUtils::GetInteger(oCSS.m_arStyles[i].m_strClassName);

				std::map<CString, CString>::iterator pPair = oCSS.m_arStyles[i].m_mapSettings.find(_T("font-index"));
				if (oCSS.m_arStyles[i].m_mapSettings.end() != pPair)
				{
					LONG lFontRef	= XmlUtils::GetInteger(pPair->second);

					size_t nCountEl = m_arElements.size();
					for (size_t j = 0; j < nCountEl; ++j)
					{
						if ((lPhType == m_arElements[j]->m_lPlaceholderType) && 
							(etShape == m_arElements[j]->m_etType))
						{
							CShapeElement* pShapeElement = dynamic_cast<CShapeElement*>(m_arElements[j]);
							if (NULL != pShapeElement)
								pShapeElement->m_oShape.m_oText.m_lFontRef = lFontRef;
						}
					}
				}

			}
		}
#endif
	}
}