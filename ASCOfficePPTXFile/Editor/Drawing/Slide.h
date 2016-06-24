/*
 * (c) Copyright Ascensio System SIA 2010-2016
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
		std::multimap<int,int>	m_mapPlaceholders;

		// размеры в миллиметрах
		long					m_lWidth;   
		long					m_lHeight; 

		// а вот эти - "настоящие" (в логической системе координат), чтобы масштабировать
		long					m_lOriginalWidth;
		long					m_lOriginalHeight;

		double					m_dStartTime;
		double					m_dEndTime;
		double					m_dDuration;

		bool					m_bIsBackground;
		CBrush					m_oBackground;

		std::vector<CColor>		m_arColorScheme;
		bool					m_bUseLayoutColorScheme;
		bool					m_bShowMasterShapes;

		CMetricInfo				m_oInfo;
		
		vector_string			m_PlaceholdersReplaceString[3];
		std::wstring			m_strComment;
		std::wstring			m_sName;
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
			m_arColorScheme.clear();
			m_arElements.clear();
			
			m_lThemeID			= -1;
			m_lLayoutID			= -1;

			m_lWidth			= 270;   
			m_lHeight			= 190; 

			m_lOriginalWidth	= 6000;
			m_lOriginalHeight	= 5000;
			
			m_dStartTime		= 0.0;
			m_dEndTime			= 0.0;
			m_dDuration			= 30000.0;

			m_bShowMasterShapes = true;
			m_strComment.clear();
			m_sName.clear();

			for (int i = 0 ; i < 3 ; i++) m_PlaceholdersReplaceString[i].clear();
		}

		CSlide(const CSlide& oSrc)
		{
			Clear();
			
			size_t nCount = oSrc.m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				m_arElements.push_back(oSrc.m_arElements[nIndex]->CreateDublicate());
			}

			m_arColorScheme		= oSrc.m_arColorScheme;

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

			for (int i = 0 ; i < 3 ; i++) m_PlaceholdersReplaceString[i] = oSrc.m_PlaceholdersReplaceString[i];

			m_strComment		= oSrc.m_strComment;
			m_sName				= oSrc.m_sName;
		}

	public:
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
		}
		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
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
								if ((pLayout->m_arElements[i]->m_lPlaceholderType	== pSlideElement->m_lPlaceholderType) &&
									(pLayout->m_arElements[i]->m_etType				== etShape))
								{
									CShapeElement* pLayoutElement = dynamic_cast<CShapeElement*>(pLayout->m_arElements[i]);
									if (NULL != pLayoutElement)
									{
										pSlideElement->m_oShape.m_oText.m_oLayoutStyles		= pLayoutElement->m_oShape.m_oText.m_oStyles;
									}
								}
							}
							else
							{
								if ((pLayout->m_arElements[i]->m_lPlaceholderType	== pSlideElement->m_lPlaceholderType) &&
									(pLayout->m_arElements[i]->m_lPlaceholderID		== pSlideElement->m_lPlaceholderID) &&
									(pLayout->m_arElements[i]->m_etType				== etShape))
								{
									CShapeElement* pLayoutElement = dynamic_cast<CShapeElement*>(pLayout->m_arElements[i]);
									if (NULL != pLayoutElement)
									{
										pSlideElement->m_oShape.m_oText.m_oLayoutStyles		= pLayoutElement->m_oShape.m_oText.m_oStyles;
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
