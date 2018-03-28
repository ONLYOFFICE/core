/*
 * (c) Copyright Ascensio System SIA 2010-2018
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
#include "Element.h"

namespace NSPresentationEditor
{
	class CLayout
	{
	public:
		std::vector<CElementPtr>m_arElements;
		std::vector<CColor>		m_arColorScheme;
		std::multimap<int,int>	m_mapPlaceholders;

		bool					m_bUseThemeColorScheme;

		// "настоящие"(в логической системе координат), чтобы масштабировать
		long					m_lOriginalWidth;
		long					m_lOriginalHeight;

		bool					m_bIsBackground;
		CBrush					m_oBackground;

		// размеры в миллиметрах
		long					m_lWidth;   
		long					m_lHeight; 	

		CMetricInfo				m_oInfo;

		bool					m_bHasDate;
		bool					m_bHasSlideNumber;
		bool					m_bHasFooter;
		int						m_nFormatDate;
		vector_string			m_PlaceholdersReplaceString[3]; //0-dates, 1 - headers, 2 - footers

		bool					m_bShowMasterShapes;
		std::wstring			m_strLayoutType;

		std::wstring			m_sName;

		CLayout() 
		{
			Clear();
		}

		CLayout(const CLayout& oSrc)
		{
			*this = oSrc;
		}

		CLayout& operator=(const CLayout& oSrc)
		{
			Clear();
		
			size_t nCount = oSrc.m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				m_arElements.push_back(oSrc.m_arElements[nIndex]->CreateDublicate());
			}

			m_mapPlaceholders		= oSrc.m_mapPlaceholders;
			m_arColorScheme			= oSrc.m_arColorScheme;

			m_bUseThemeColorScheme	= oSrc.m_bUseThemeColorScheme;

			m_lOriginalWidth		= oSrc.m_lOriginalWidth;
			m_lOriginalHeight		= oSrc.m_lOriginalHeight;

			m_lWidth				= oSrc.m_lWidth;   
			m_lHeight				= oSrc.m_lHeight; 

			m_bIsBackground			= oSrc.m_bIsBackground;
			m_oBackground			= oSrc.m_oBackground;

			m_bHasDate				= oSrc.m_bHasDate;
			m_bHasSlideNumber		= oSrc.m_bHasSlideNumber;
			m_bHasFooter			= oSrc.m_bHasFooter;
			m_nFormatDate			= oSrc.m_nFormatDate;

			for (int i = 0 ; i < 3 ; i++) 
				m_PlaceholdersReplaceString[i] = oSrc.m_PlaceholdersReplaceString[i];

			m_bShowMasterShapes		= oSrc.m_bShowMasterShapes;
			m_strLayoutType			= oSrc.m_strLayoutType;

			m_sName					= oSrc.m_sName;

			return *this;
		}

		void SetMetricInfo(const CMetricInfo& oInfo)
		{
			m_oInfo  = oInfo;
			m_lWidth			= m_oInfo.m_lMillimetresHor;
			m_lHeight			= m_oInfo.m_lMillimetresVer;
			m_lOriginalWidth	= m_oInfo.m_lUnitsHor;
			m_lOriginalHeight	= m_oInfo.m_lUnitsVer;
		}

		void Clear()
		{
			m_arElements.clear();
			m_mapPlaceholders.clear();
			
			m_bHasDate			= false;
			m_bHasSlideNumber	= false;
			m_bHasFooter		= false;
			m_nFormatDate		= 1;
			
			for (int i = 0 ; i < 3 ; i++) 
				m_PlaceholdersReplaceString[i].clear();

			m_bUseThemeColorScheme	= true;
			m_bShowMasterShapes		= true;
			m_strLayoutType			= _T("obj");
			m_bIsBackground			= false;

			m_lWidth = m_lHeight = m_lOriginalWidth = m_lOriginalHeight = 0;
		}

		void CreateDublicateElements()
		{
			// просто из всех своих элементов делаем дубликата

			for (size_t nIndex = 0; nIndex < m_arElements.size(); ++nIndex)
			{
				CElementPtr pElem = m_arElements[nIndex];
				if (NULL != pElem)
				{
					m_arElements[nIndex] = pElem->CreateDublicate();
				}
			}
		}

		CLayout* CreateDublicate()
		{
			CLayout* pNew = new CLayout(*this);
			pNew->CreateDublicateElements();
			return pNew;
		}

		CElementPtr GetPlaceholder(LONG lID)
		{
			size_t nCount = m_arElements.size();
			
			for (size_t i = 0; i < nCount; ++i)
			{
				CElementPtr pElem = m_arElements[i];

				if (pElem->m_lPlaceholderType == lID)
				{
					return pElem;
				}
			}
			
			return CElementPtr();
		}

		LONG GetCountPlaceholderWithType(LONG lType)
		{
			LONG lFound = 0;

			for (size_t i = 0; i < m_arElements.size(); ++i)
			{
				if (m_arElements[i]->m_lPlaceholderType == lType)
				{
					++lFound;
				}
			}
			
			return lFound;
		}
		NSPresentationEditor::CColor GetColor(const LONG& lIndexScheme)
		{
			if (lIndexScheme < (LONG)m_arColorScheme.size())
			{
				return m_arColorScheme[lIndexScheme];
			}
			return NSPresentationEditor::CColor();
		}

        static void CheckPlaceholderStyle(std::wstring strStyleName, LONG& lType, LONG& lLevel, LONG& lTypeStyle)
		{
            size_t		nLen  = strStyleName.length();
            wchar_t*    pData = (wchar_t*)strStyleName.c_str();

			lType		= 0;
			lLevel		= 0;
			lTypeStyle	= -1;

            wchar_t* pDataEnd = pData + nLen;
            wchar_t* pDataMem = pData + 1;
			for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
			{
				lType *= 10;
                lType += (*pDataMem - ((wchar_t)'0'));
			}

			pDataMem += 4;
			for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
			{
				lLevel *= 10;
                lLevel += (*pDataMem - ((wchar_t)'0'));
			}

			++pDataMem;
			if (pDataMem + 1 < pDataEnd)
			{
                if ((wchar_t('c') == pDataMem[0]) && (wchar_t('f') == pDataMem[1]))
					lTypeStyle = 1;
                if ((wchar_t('p') == pDataMem[0]) && (wchar_t('f') == pDataMem[1]))
					lTypeStyle = 0;
			}
		}
	};
}
