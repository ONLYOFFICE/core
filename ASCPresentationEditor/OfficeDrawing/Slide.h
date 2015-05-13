#pragma once
#include "SlideShow.h"
#include "Theme.h"

namespace NSPresentationEditor
{
	class CSlide
	{
	public:
		LONG m_lThemeID;
		LONG m_lLayoutID;

		std::vector<IElement*>	m_arElements;
		CSlideShowInfo			m_oSlideShow;

		// размеры в миллиметрах
		long m_lWidth;   
		long m_lHeight; 

		// а вот эти - "настоящие" (в логической системе координат), чтобы масштабировать
		long m_lOriginalWidth;
		long m_lOriginalHeight;

		double m_dStartTime;
		double m_dEndTime;
		double m_dDuration;

		bool m_bIsBackground;
		CBrush m_oBackground;

		std::vector<CColor>		m_arColorScheme;
		bool					m_bUseLayoutColorScheme;
		bool					m_bShowMasterShapes;

		CMetricInfo				m_oInfo;
		
		CString					m_strComment;

	public:
		CSlide() : m_arElements(), m_oSlideShow()
		{
			Clear(); 
		}
		~CSlide()
		{
			Clear();
		}

		void Clear()
		{
			for (size_t nIndex = 0; nIndex < m_arElements.size(); ++nIndex)
			{
				IElement* pElem = m_arElements[nIndex];
				RELEASEINTERFACE(pElem);
			}
			m_arElements.clear();
			
			m_lThemeID		= -1;
			m_lLayoutID		= -1;

			m_lWidth		= 270;   
			m_lHeight		= 190; 

			m_lOriginalWidth	= 6000;
			m_lOriginalHeight	= 5000;
			
			m_dStartTime	= 0.0;
			m_dEndTime		= 0.0;
			m_dDuration		= 30000.0;

			m_bShowMasterShapes = true;
			m_strComment = _T("");

		}

		CSlide& operator=(const CSlide& oSrc)
		{
			Clear();
			
			size_t nCount = oSrc.m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				m_arElements.push_back(oSrc.m_arElements[nIndex]->CreateDublicate());
			}

			m_oSlideShow		= oSrc.m_oSlideShow;

			m_lThemeID			= oSrc.m_lThemeID;
			m_lLayoutID			= oSrc.m_lLayoutID;

			m_lWidth			= oSrc.m_lWidth;
			m_lHeight			= oSrc.m_lHeight;

			m_lOriginalWidth	= oSrc.m_lOriginalWidth;
			m_lOriginalHeight	= oSrc.m_lOriginalHeight;
			
			m_dStartTime		= oSrc.m_dStartTime;
			m_dEndTime			= oSrc.m_dEndTime;
			m_dDuration			= oSrc.m_dDuration;

			m_bIsBackground		= oSrc.m_bIsBackground;
			m_oBackground		= oSrc.m_oBackground;

			m_bShowMasterShapes = oSrc.m_bShowMasterShapes;

			m_strComment		= oSrc.m_strComment;
			return *this;
		}

		CSlide(const CSlide& oSrc)
		{
			Clear();
			
			size_t nCount = oSrc.m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				m_arElements.push_back(oSrc.m_arElements[nIndex]->CreateDublicate());
			}

			m_oSlideShow		= oSrc.m_oSlideShow;

			m_lThemeID			= oSrc.m_lThemeID;
			m_lLayoutID			= oSrc.m_lLayoutID;

			m_lWidth			= oSrc.m_lWidth;
			m_lHeight			= oSrc.m_lHeight;

			m_lOriginalWidth	= oSrc.m_lOriginalWidth;
			m_lOriginalHeight	= oSrc.m_lOriginalHeight;
			
			m_dStartTime		= oSrc.m_dStartTime;
			m_dEndTime			= oSrc.m_dEndTime;
			m_dDuration			= oSrc.m_dDuration;

			m_bIsBackground		= oSrc.m_bIsBackground;
			m_oBackground		= oSrc.m_oBackground;

			m_bShowMasterShapes = oSrc.m_bShowMasterShapes;

			m_strComment		= oSrc.m_strComment;
		}

	public:
		void Calculate(CTheme* pTheme)
		{
			CLayout* pLayout = NULL;

			if (NULL != pTheme)
			{
				if ((0 <= m_lLayoutID) && (m_lLayoutID < (LONG)pTheme->m_arLayouts.size()))
				{
					pLayout = &pTheme->m_arLayouts[m_lLayoutID];
				}
			}

			// все элементы добавлены
			size_t nCount = m_arElements.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				CalculateElement(i, pTheme, pLayout);
				m_arElements[i]->m_pTheme = pTheme;
			}
		}

		void ClearPreset()
		{
			LONG nCount = (LONG)m_arElements.size();
			for (LONG i = 0; i < nCount; ++i)
			{
				IElement* pElement = m_arElements[i];

				if (NULL == pElement)
				{
					m_arElements.erase(m_arElements.begin() + i);
					--i;
					--nCount;
					continue;
				}
				else if (pElement->m_bIsLayoutElement)
				{
					m_arElements.erase(m_arElements.begin() + i);
					--i;
					--nCount;
					RELEASEINTERFACE(pElement);
				}
			}
		}

		void CalculateElement(size_t nIndexElement, CTheme* pTheme, CLayout* pLayout)
		{
			IElement* pElement = m_arElements[nIndexElement];
			if (NULL == pElement)
				return;

			LONG lPlaceholderID = pElement->m_lPlaceholderType;
			if (-1 == lPlaceholderID)
			{
				//pElement->SetupProperties(pTheme, pColorScheme);
				m_arElements[nIndexElement]->SetupProperties(this, pTheme, pLayout);
				return;
			}

			IElement* pLayoutElem = pLayout->GetPlaceholder(lPlaceholderID, FALSE);

			if (NULL == pLayoutElem)
			{
				m_arElements[nIndexElement]->SetupProperties(this, pTheme, pLayout);
				return;
			}
			
			//m_arElements[nIndexElement] = pLayoutElem->CreateDublicate();
			//m_arElements[nIndexElement]->m_oProperties = pElement->m_oProperties;
			m_arElements[nIndexElement]->SetupProperties(this, pTheme, pLayout);
		}

		virtual CString SerializeToXml()
		{
			CString strSlide = _T("");
			strSlide.Format(_T("<Slide tid='%d' lid='%d'>"), m_lThemeID, m_lLayoutID);
			for (size_t i = 0; i < m_arElements.size(); ++i)
			{
				strSlide += m_arElements[i]->SerializeToXml();
			}
			strSlide += _T("</Slide>");
			return strSlide;
		}

		void SetMetricInfo(const CMetricInfo& oInfo)
		{
			m_oInfo  = oInfo;
			m_lWidth			= m_oInfo.m_lMillimetresHor;
			m_lHeight			= m_oInfo.m_lMillimetresVer;
			m_lOriginalWidth	= m_oInfo.m_lUnitsHor;
			m_lOriginalHeight	= m_oInfo.m_lUnitsVer;
		}

		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode)
		{
#ifdef _PRESENTATION_WRITER_
			m_bUseLayoutColorScheme = true;
			//colors
			m_arColorScheme.clear();
			XmlUtils::CXmlNode oNodeColors;
			if (oNode.GetNode(_T("Colors"), oNodeColors))
			{
				m_bUseLayoutColorScheme = false;

                CStylesCSS  oStyles;
                CString     strStyles = oNodeColors.GetText();

                oStyles.LoadStyles(strStyles);

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
			XmlUtils::CXmlNodes oNodes;
			if (oNode.GetNodes(_T("Element"), oNodes))
			{
				int nCount = oNodes.GetCount();
				for (int i = 0; i < nCount; ++i)
				{
					XmlUtils::CXmlNode oNodeMem;
					oNodes.GetAt(i, oNodeMem);

					CShapeElement* pShapeEl = new CShapeElement();
					pShapeEl->m_oMetric = m_oInfo;
					pShapeEl->LoadFromXmlNode(oNodeMem);

					pShapeEl->m_lID					= oNodeMem.ReadAttributeInt(_T("id"), -1);
					pShapeEl->m_lLayoutID			= oNodeMem.ReadAttributeInt(_T("layoutid"), -1);
					pShapeEl->m_lPlaceholderID		= oNodeMem.ReadAttributeInt(_T("phid"), -1);
					pShapeEl->m_lPlaceholderType	= oNodeMem.ReadAttributeInt(_T("phtype"), -1);

					pShapeEl->m_oShape.m_oText.m_lPlaceholderType	= pShapeEl->m_lPlaceholderType;

					pShapeEl->m_dRotate				= oNodeMem.ReadAttributeDouble(_T("angle"));
					pShapeEl->m_bFlipH				= (1 == oNodeMem.ReadAttributeInt(_T("flipx")));
					pShapeEl->m_bFlipV				= (1 == oNodeMem.ReadAttributeInt(_T("flipy")));

					m_arElements.push_back(pShapeEl);
				}
			}

			XmlUtils::CXmlNode oNodeFontRefs;
			if (oNode.GetNode(_T("StylesFontRef"), oNodeFontRefs))
			{
                CStylesCSS  oCSS;
                CString     strStyles = oNodeFontRefs.GetText();

                oCSS.LoadStyles(strStyles);

				size_t nCountS = oCSS.m_arStyles.size();
				for (size_t i = 0; i < nCountS; ++i)
				{
					oCSS.m_arStyles[i].m_strClassName.Delete(0, 6);
					LONG lElementID	= XmlUtils::GetInteger(oCSS.m_arStyles[i].m_strClassName);

					std::map<CString, CString>::iterator pPair = oCSS.m_arStyles[i].m_mapSettings.find(_T("font-index"));
					if (oCSS.m_arStyles[i].m_mapSettings.end() != pPair)
					{
						LONG lFontRef	= XmlUtils::GetInteger(pPair->second);

						size_t nCountEl = m_arElements.size();
						for (size_t j = 0; j < nCountEl; ++j)
						{
							if ((lElementID == m_arElements[j]->m_lID) && 
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

			XmlUtils::CXmlNode oNodeComment;
			if (oNode.GetNode(_T("Comment"), oNodeComment))
			{
				m_strComment = oNodeComment.GetTextExt();
			}
#endif
		}
		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
		}

		CString ToXmlEditor(NSPresentationEditor::CTheme* pTheme, NSPresentationEditor::CLayout* pLayout, const NSPresentationEditor::CMetricInfo& oInfo)
		{
			NSPresentationEditor::CStringWriter oWriter;
						
			LONG lWidthPix		= (LONG)(96.0 * m_lWidth  / 25.4);
			LONG lHeightPix		= (LONG)(96.0 * m_lHeight / 25.4);

			CString XmlSlideSource = _T("");
			XmlSlideSource.Format ( _T("<Slide color=\"16777215\" duration=\"%lf\" width=\"%d\" height=\"%d\" themeid=\"%d\" layoutid=\"%d\" themeidsrc=\"%d\" layoutidsrc=\"%d\" usethemeobjects=\"%d\" transition=\"%d\" transition-speed=\"%.2lf\">"), 
									m_dDuration, lWidthPix, lHeightPix, m_lThemeID, m_lLayoutID, m_lThemeID, m_lLayoutID, m_bShowMasterShapes ? 1 : 0, 
									m_oSlideShow.m_oTransition.m_nEffectType, m_oSlideShow.m_oTransition.m_dSpeed );

			oWriter.WriteString(XmlSlideSource);

			if (m_bIsBackground)
			{
				// background
				#ifdef PPT_DEF
				CShapeElement oElem(NSPresentationEditor::NSBaseShape::ppt, PPTShapes::sptCRect);
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

				oElem.NormalizeCoords(dScaleX, dScaleY);
				oElem.m_oShape.m_oBrush = m_oBackground;

				oElem.m_bIsBackground = true;
				oElem.m_bIsChangeable = false;

				oWriter.WriteString(_T("<Background>"));
				oElem.SetupProperties(this, pTheme, pLayout);
				oWriter.WriteString(oElem.ToXmlEditor());
				oWriter.WriteString(_T("</Background>"));
			}

			CString strFontRefs = _T("");

			// elements
			size_t nCount = m_arElements.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				IElement* pElement = m_arElements[i];

				CString strElement = _T("");
				strElement.Format(_T("<Element type=\"%d\" id=\"%d\" layoutid=\"%d\" phid=\"%d\" phtype=\"%d\" angle=\"%lf\" flipx=\"%d\" flipy=\"%d\">"), 
					(int)pElement->m_etType, pElement->m_lID, pElement->m_lLayoutID, pElement->m_lPlaceholderID, 
					pElement->m_lPlaceholderType, pElement->m_dRotate, pElement->m_bFlipH ? 1 : 0, pElement->m_bFlipV ? 1 : 0);

				pElement->m_bIsBackground = false;
				pElement->m_bIsChangeable = true;

				oWriter.WriteString(strElement);
				oWriter.WriteString(pElement->ToXmlEditor());
				oWriter.WriteString(_T("</Element>"));

				if (etShape == pElement->m_etType)
				{
					CShapeElement* pTextElement = dynamic_cast<CShapeElement*>(pElement);
					if (NULL != pTextElement)
					{
						LONG lFontRef = pTextElement->m_oShape.m_oText.m_lFontRef;
						if (pTextElement->m_oShape.m_oText.m_bIsSlideFontRef && (0 <= lFontRef) && (lFontRef < (LONG)pTheme->m_arFonts.size()))
						{
							CString strRef = _T("");
                            strRef.Format(_T("s_font%d { font-index:%d;font-family:%ls; }\n"),
								pTextElement->m_lID, lFontRef, pTheme->m_arFonts[lFontRef].Name);
							strFontRefs += strRef;
						}
					}
				}
			}

			oWriter.WriteString(_T("<StylesFontRef>") + strFontRefs + _T("</StylesFontRef>"));

			oWriter.WriteString(_T("</Slide>"));

			return oWriter.GetData();
		}

		void SetUpPlaceholderStyles(NSPresentationEditor::CLayout* pLayout)
		{
			size_t nCountElements = m_arElements.size();
			for (size_t nEl = 0; nEl < nCountElements; ++nEl)
			{
				if (-1 != m_arElements[nEl]->m_lPlaceholderType && etShape == m_arElements[nEl]->m_etType)
				{
					CShapeElement* pSlideElement = dynamic_cast<CShapeElement*>(m_arElements[nEl]);

					if (NULL != pSlideElement)
					{
						LONG lCountThisType = pLayout->GetCountPlaceholderWithType(pSlideElement->m_lPlaceholderType);

						size_t nCountLayout = pLayout->m_arElements.size();
						for (size_t i = 0; i < nCountLayout; ++i)
						{
							if (1 == lCountThisType)
							{
								if ((pLayout->m_arElements[i]->m_lPlaceholderType == pSlideElement->m_lPlaceholderType) &&
									(pLayout->m_arElements[i]->m_etType == etShape))
								{
									CShapeElement* pLayoutElement = dynamic_cast<CShapeElement*>(pLayout->m_arElements[i]);
									if (NULL != pLayoutElement)
									{
										pSlideElement->m_oShape.m_oText.m_oLayoutStyles = pLayoutElement->m_oShape.m_oText.m_oStyles;
									}
								}
							}
							else
							{
								if ((pLayout->m_arElements[i]->m_lPlaceholderType == pSlideElement->m_lPlaceholderType) &&
									(pLayout->m_arElements[i]->m_lPlaceholderID == pSlideElement->m_lPlaceholderID) &&
									(pLayout->m_arElements[i]->m_etType == etShape))
								{
									CShapeElement* pLayoutElement = dynamic_cast<CShapeElement*>(pLayout->m_arElements[i]);
									if (NULL != pLayoutElement)
									{
										pSlideElement->m_oShape.m_oText.m_oLayoutStyles = pLayoutElement->m_oShape.m_oText.m_oStyles;
									}
								}								
							}
						}
					}
				}
			}
		}

		NSPresentationEditor::CColor GetColor(const LONG& lIndexScheme)
		{
			if (lIndexScheme < (LONG)m_arColorScheme.size())
			{
				return m_arColorScheme[lIndexScheme];
			}
			return NSPresentationEditor::CColor();
		}
	};
}
