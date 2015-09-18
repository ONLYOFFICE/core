#include "Elements.h"

namespace NSPresentationEditor
{
	CString NSPresentationEditor::CTheme::ToXmlEditor(const CMetricInfo& oInfo)
	{
		NSPresentationEditor::CStringWriter oWriter;

		oWriter.WriteString(std::wstring(L"<Theme name=\""));
		oWriter.WriteString(m_sThemeName);
		oWriter.WriteString(std::wstring(L"\">"));

		// colors ----
		oWriter.WriteString(std::wstring(L"<Colors>"));
		
		size_t nCountColors = m_arColorScheme.size();
		for (size_t i = 0; i < nCountColors; ++i)
		{
			CString strFormat = _T("");
			LONG lColorValue = m_arColorScheme[i].GetLONG_RGB();
			strFormat.Format(_T(".color%u { color: #%06x; } .stroke%u { stroke:#%06x; } .fill%u { fill:#%06x; }\n"), 
				i + 1, lColorValue, i + 1, lColorValue, i + 1, lColorValue);
			oWriter.WriteString(strFormat);
		}

		oWriter.WriteString(std::wstring(L"</Colors>"));
		// -----------
		
		// fonts -----
		oWriter.WriteString(std::wstring(L"<Fonts>"));
		
		size_t nCountFonts = m_arFonts.size();
		//if (nCountFonts > 1)
		//	nCountFonts = 1;
		for (size_t i = 0; i < nCountFonts; ++i)
		{
			CString strFormat = _T("");

            strFormat.Format(_T(".font%u { font-family:%ls;tmdocs_charset:%d;tmdocs_monospace:%d;tmdocs_panose:%ls; }\n"),
				i + 1, m_arFonts[i].Name, m_arFonts[i].Charset, m_arFonts[i].Monospace, m_arFonts[i].Panose);

			oWriter.WriteString(strFormat);
		}

		oWriter.WriteString(std::wstring(L"</Fonts>"));

		// -----------
		
		// fonts dublicate
		oWriter.WriteString(std::wstring(L"<FontsDublicate>"));
		for (size_t i = 0; i < nCountFonts; ++i)
		{
			CString strFormat = _T("");

            strFormat.Format(_T(".font_d%u { font-family:%ls; }\n"), i + 1, m_arFonts[i].Name);

			oWriter.WriteString(strFormat);
		}
		oWriter.WriteString(std::wstring(L"</FontsDublicate>"));
		// -----------

		// textstyles ------
		oWriter.WriteString(std::wstring(L"<TextStyles>"));
		
		for (long i = 0; i < g_ThemeTextStylesCount; ++i)
		{
			oWriter.WriteString(m_pStyles[i].ToXmlEditor(oInfo, false, i, -1));
		}
		
		oWriter.WriteString(std::wstring(L"</TextStyles>"));
		// -----------------

		// background
		#ifndef ENABLE_PPT_TO_PPTX_CONVERT
			#ifdef PPT_DEF
				CShapeElement oElem(NSPresentationEditor::NSBaseShape::ppt, PPTShapes::sptCRect);
			#else
				CShapeElement oElem(NSPresentationEditor::NSBaseShape::pptx, OOXMLShapes::sptCRect);
			#endif
		#else
			CShapeElement oElem(NSPresentationEditor::NSBaseShape::pptx, OOXMLShapes::sptCRect);
		#endif

		oElem.m_oMetric = oInfo;
		oElem.m_rcBoundsOriginal.left	= 0.0;
		oElem.m_rcBoundsOriginal.right	= (double)oInfo.m_lUnitsHor;
		oElem.m_rcBoundsOriginal.top	= 0.0;
		oElem.m_rcBoundsOriginal.bottom	= (double)oInfo.m_lUnitsVer;

		double dScaleX = (double)oInfo.m_lMillimetresHor / oInfo.m_lUnitsHor;
		double dScaleY = (double)oInfo.m_lMillimetresVer / oInfo.m_lUnitsVer;

		oElem.m_bIsBackground = true;
		oElem.m_bIsChangeable = false;

		oElem.NormalizeCoords(dScaleX, dScaleY);
		oElem.m_oShape.m_oBrush = m_oBackground;

		oWriter.WriteString(std::wstring(L"<Background>"));
		oElem.SetupProperties(NULL, this, NULL);
		oWriter.WriteString(oElem.ToXmlEditor());
		oWriter.WriteString(std::wstring(L"</Background>"));
		// -----------
		// elements
		oWriter.WriteString(std::wstring(L"<Elements>"));

		size_t nCountElems = m_arElements.size();
		for (size_t i = 0; i < nCountElems; ++i)
		{
			m_arElements[i]->m_bIsBackground = false;
			m_arElements[i]->m_bIsChangeable = false;
			
			oWriter.WriteString(m_arElements[i]->ToXmlEditor());
		}

		oWriter.WriteString(std::wstring(L"</Elements>"));
		// ----------------
		// layouts
		oWriter.WriteString(std::wstring(L"<Layouts>"));

		size_t nCountLayouts = m_arLayouts.size();
		for (size_t i = 0; i < nCountLayouts; ++i)
		{
			oWriter.WriteString(m_arLayouts[i].ToXmlEditor(this, oInfo));
		}

		oWriter.WriteString(std::wstring(L"</Layouts>"));

		oWriter.WriteString(std::wstring(L"</Theme>"));

		return oWriter.GetData();
	}

	void NSPresentationEditor::CTheme::ReadFromXml(XmlUtils::CXmlNode& oNode)
	{
#ifdef _PRESENTATION_WRITER_
		//colors
		m_sThemeName = oNode.GetAttribute(L"name", L"default");
	
		m_arColorScheme.clear();
		XmlUtils::CXmlNode oNodeColors;
		if (oNode.GetNode(_T("Colors"), oNodeColors))
		{
            CString strNodeColor = oNodeColors.GetText();

            CStylesCSS oStyles;
            oStyles.LoadStyles(strNodeColor);

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

		XmlUtils::CXmlNode oNodeFonts;
		if (oNode.GetNode(_T("Fonts"), oNodeFonts))
		{
            CString strNodeFonts = oNodeFonts.GetText();

            CStylesCSS oStyles;
            oStyles.LoadStyles(strNodeFonts);

			size_t nCount = oStyles.m_arStyles.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				CFont elem;
				m_arFonts.push_back(elem);
				oStyles.m_arStyles[i].LoadFont(m_arFonts[i]);
			}
		}

		XmlUtils::CXmlNode oNodeTextStyles;
		if (oNode.GetNode(_T("TextStyles"), oNodeTextStyles))
		{
			XmlUtils::CXmlNodes oStyles;
			if (oNodeTextStyles.GetNodes(_T("Style"), oStyles))
			{
				int nCount = oStyles.GetCount();

				if (nCount >= g_ThemeTextStylesCount)
				{
					for (int i = 0; i < g_ThemeTextStylesCount; ++i)
					{
						XmlUtils::CXmlNode oNodeStyle;
						oStyles.GetAt(i, oNodeStyle);

						int nNumberStyle = oNodeStyle.ReadAttributeInt(_T("type"));

                        CString strNodeStyle = oNodeStyle.GetText();

                        CStylesCSS oCSS;
                        oCSS.LoadStyles(strNodeStyle);

                        if (20 != oCSS.m_arStyles.size())
							continue;

						for (int nIndexStyle = 0; nIndexStyle < 10; ++nIndexStyle)
						{
							m_pStyles[i].m_pLevels[nIndexStyle] = new CTextStyleLevel();
							
							oCSS.m_arStyles[2 * nIndexStyle].LoadStylePF(m_pStyles[i].m_pLevels[nIndexStyle]->m_oPFRun, m_oInfo);
							oCSS.m_arStyles[2 * nIndexStyle + 1].LoadStyleCF(m_pStyles[i].m_pLevels[nIndexStyle]->m_oCFRun, m_oInfo);
						}
					}
				}
			}
		}

		// background
		m_bIsBackground = false;
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

					pShapeEl->m_pTheme = this;

					m_arElements.push_back(pShapeEl);
				}
			}
		}

		XmlUtils::CXmlNode oNodeLayouts;
		if (oNode.GetNode(_T("Layouts"), oNodeLayouts))
		{
			XmlUtils::CXmlNodes oLayouts;
			if (oNodeLayouts.GetNodes(_T("Layout"), oLayouts))
			{
				int nCountLayouts = oLayouts.GetCount();
				for (int nL = 0; nL < nCountLayouts; ++nL)
				{
					XmlUtils::CXmlNode oNodeL;
					oLayouts.GetAt(nL, oNodeL);
					
					CLayout elem;
					m_arLayouts.push_back(elem);
					
					m_arLayouts.back().SetMetricInfo(m_oInfo);
					m_arLayouts.back().ReadFromXml(oNodeL);
				}
			}
		}
#endif
	}
}
