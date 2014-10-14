#pragma once
#include "Base.h"
#include "Element.h"

namespace NSPresentationEditor
{
	class CLayout : public IBase
	{
	public:
		std::vector<IElement*>	m_arElements;
		std::vector<CColor>		m_arColorScheme;
		bool					m_bUseThemeColorScheme;

		// "насто€щие"(в логической системе координат), чтобы масштабировать
		long m_lOriginalWidth;
		long m_lOriginalHeight;

		bool m_bIsBackground;
		CBrush m_oBackground;

		// размеры в миллиметрах
		long m_lWidth;   
		long m_lHeight; 	

		CMetricInfo m_oInfo;

		bool					m_bShowMasterShapes;
		CString					m_strLayoutType;

	public:
		CLayout() : m_arElements(), m_arColorScheme()
		{
			m_bUseThemeColorScheme	= true;
			m_bShowMasterShapes		= true;
			m_strLayoutType			= _T("title");
		}

		CLayout(const CLayout& oSrc)
		{
			*this = oSrc;
		}

		CLayout& operator=(const CLayout& oSrc)
		{
			Clear();
			m_arElements.insert(m_arElements.end(), oSrc.m_arElements.begin(), oSrc.m_arElements.end());

			size_t nCount = m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
			{
				ADDREFINTERFACE((m_arElements[nIndex]));
			}

			m_arColorScheme.insert(m_arColorScheme.end(), oSrc.m_arColorScheme.begin(), oSrc.m_arColorScheme.end());

			m_bUseThemeColorScheme	= oSrc.m_bUseThemeColorScheme;

			m_lOriginalWidth		= oSrc.m_lOriginalWidth;
			m_lOriginalHeight		= oSrc.m_lOriginalHeight;

			m_lWidth				= oSrc.m_lWidth;   
			m_lHeight				= oSrc.m_lHeight; 

			m_bIsBackground			= oSrc.m_bIsBackground;
			m_oBackground			= oSrc.m_oBackground;

			m_bShowMasterShapes		= oSrc.m_bShowMasterShapes;
			m_strLayoutType			= oSrc.m_strLayoutType;

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
		}

		void CreateDublicateElements()
		{
			// просто из всех своих элементов делаем дубликата
			size_t nCount = m_arElements.size();
			for (size_t nIndex = 0; nIndex < nCount; ++nIndex)
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

		IElement* GetPlaceholder(LONG lID, BOOL bIsAddRef)
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
			size_t nCount = m_arElements.size();
			for (size_t i = 0; i < nCount; ++i)
			{
				if (m_arElements[i]->m_lPlaceholderType == lType)
				{
					++lFound;
				}
			}
			
			return lFound;
		}

		CString ToXmlEditor(NSPresentationEditor::CTheme* pTheme, const NSPresentationEditor::CMetricInfo& oInfo);

	public:
		virtual void ReadFromXml(XmlUtils::CXmlNode& oNode);
		virtual void WriteToXml(XmlUtils::CXmlWriter& oWriter)
		{
		}

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
