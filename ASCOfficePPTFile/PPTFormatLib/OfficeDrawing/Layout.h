#pragma once
#include "Element.h"

namespace NSPresentationEditor
{
	class CLayout
	{
	public:
		std::vector<IElement*>	m_arElements;
		std::vector<CColor>		m_arColorScheme;
		std::multimap<int,int>	m_mapPlaceholders;

		bool					m_bUseThemeColorScheme;

		// "насто€щие"(в логической системе координат), чтобы масштабировать
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
	public:
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
		
			m_arElements			= oSrc.m_arElements;

			size_t nCount = m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				ADDREFINTERFACE((m_arElements[nIndex]));
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

			for (int i = 0 ; i < 3 ; i++) m_PlaceholdersReplaceString[i] = oSrc.m_PlaceholdersReplaceString[i];

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

	public:
		void Clear()
		{
			size_t nCount = m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				RELEASEINTERFACE((m_arElements[nIndex]));
			}

			m_arElements.clear();
			m_mapPlaceholders.clear();
			
			m_bHasDate			= false;
			m_bHasSlideNumber	= false;
			m_bHasFooter		= false;
			m_nFormatDate		= 1;
			for (int i = 0 ; i < 3 ; i++) m_PlaceholdersReplaceString[i].clear();

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
				IElement* pElem = m_arElements[nIndex];
				if (NULL != pElem)
				{
					m_arElements[nIndex] = pElem->CreateDublicate();
				}
				RELEASEINTERFACE(pElem);
			}
		}

		CLayout* CreateDublicate()
		{
			CLayout* pNew = new CLayout(*this);
			pNew->CreateDublicateElements();
			return pNew;
		}

		IElement* GetPlaceholder(LONG lID, bool bIsAddRef)
		{
			size_t nCount = m_arElements.size();
			
			for (size_t i = 0; i < nCount; ++i)
			{
				IElement* pElem = m_arElements[i];

				if (pElem->m_lPlaceholderType == lID)
				{
					if (bIsAddRef)
						ADDREFINTERFACE(pElem);

					return pElem;
				}
			}
			
			return NULL;
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

	public:


		NSPresentationEditor::CColor GetColor(const LONG& lIndexScheme)
		{
			if (lIndexScheme < (LONG)m_arColorScheme.size())
			{
				return m_arColorScheme[lIndexScheme];
			}
			return NSPresentationEditor::CColor();
		}

		static void CheckPlaceholderStyle(CString strStyleName, LONG& lType, LONG& lLevel, LONG& lTypeStyle)
		{
			int nLen = strStyleName.GetLength();
			TCHAR* pData = strStyleName.GetBuffer();

			lType		= 0;
			lLevel		= 0;
			lTypeStyle	= -1;

			TCHAR* pDataEnd = pData + nLen;
			TCHAR* pDataMem = pData + 1;
			for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
			{
				lType *= 10;
				lType += (*pDataMem - ((TCHAR)'0'));
			}

			pDataMem += 4;
			for (; ((pDataMem < pDataEnd) && XmlUtils::IsDigit(*pDataMem)); ++pDataMem)
			{
				lLevel *= 10;
				lLevel += (*pDataMem - ((TCHAR)'0'));
			}

			++pDataMem;
			if (pDataMem + 1 < pDataEnd)
			{
				if ((TCHAR('c') == pDataMem[0]) && (TCHAR('f') == pDataMem[1]))
					lTypeStyle = 1;
				if ((TCHAR('p') == pDataMem[0]) && (TCHAR('f') == pDataMem[1]))
					lTypeStyle = 0;
			}
		}
	};
}
