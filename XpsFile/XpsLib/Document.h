#ifndef _XPS_XPSLIB_DOCUMENT_H
#define _XPS_XPSLIB_DOCUMENT_H

#include "FontList.h"
#include "Page.h"
#include <map>
#include <vector>

#define UNICODE
#define _UNICODE
#define _USE_LIBXML2_READER_
#define LIBXML_READER_ENABLED

#include "../../DesktopEditor/graphics/IRenderer.h"
#include "../../DesktopEditor/graphics/TemporaryCS.h"

namespace XPS
{
	class CPath;
	class CStaticResource;
	class CDocument
	{
	public:
		CDocument(CFontManager* pFontManager);
		~CDocument();

		bool ReadFromPath(const std::wstring& wsPath);
		int  GetPageCount() const;
		void GetPageSize(int nPageIndex, int& nW, int& nH);
		void DrawPage(int nPageIndex, IRenderer* pRenderer, bool* pbBreak);
		void Close();
		CStaticResource* GetStaticResource(const std::wstring& wsPath);

	private:
									        
		std::wstring                             m_wsPath;					      
		std::map<int, XPS::Page*>                m_mPages;
		CFontList                                m_oFontList;
		CFontManager*                            m_pFontManager;
		std::map<std::wstring, CStaticResource*> m_mStaticResources;
	};

	class CStaticResource
	{
	public:
		CStaticResource(const std::wstring& wsPath)
		{
			clock_t oBeginTime = clock();

			XmlUtils::CXmlLiteReader oReader;

			if (!oReader.FromFile(wsPath))
				return;

			Parse(oReader);

			clock_t oEndTime = clock();
			double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
			printf("\n\nSTATIC RESOURCE %S %fseconds\n\n", wsPath.c_str() , dElapsedSecs);
		}
		CStaticResource(XmlUtils::CXmlLiteReader& oReader)
		{
			if (oReader.IsEmptyNode())
				return;

			Parse(oReader);
		}
		~CStaticResource()
		{
		}
		const wchar_t* Get(const wchar_t* wsKey)
		{
			CWString _wsKey((wchar_t*)wsKey, false);
			std::map<CWString, CWString>::iterator oIter = m_mFigures.find(_wsKey);
			if (oIter != m_mFigures.end())
				return oIter->second.c_str();

			return NULL;
		}
		const wchar_t* Get(CWString wsKey)
		{
			std::map<CWString, CWString>::iterator oIter = m_mFigures.find(wsKey);
			if (oIter != m_mFigures.end())
				return oIter->second.c_str();

			return NULL;
		}

	private:

		void Parse(XmlUtils::CXmlLiteReader& oReader)
		{
			CWString wsNodeName;
			CWString wsAttrName;
			int nCurDepth = oReader.GetDepth();
			while (oReader.ReadNextSiblingNode(nCurDepth))
			{
				wsNodeName = oReader.GetName();
				if (wsNodeName == L"PathGeometry")
				{
					CWString wsKey, wsValue;

					if (oReader.MoveToFirstAttribute())
					{
						wsAttrName = oReader.GetName();
						while (!wsAttrName.empty())
						{
							if (wsAttrName == L"x:Key")
								wsKey.create(oReader.GetText(), true);
							else if (wsAttrName == L"Figures")
								wsValue.create(oReader.GetText(), true);

							if (!oReader.MoveToNextAttribute())
								break;

							wsAttrName = oReader.GetName();
						}

						oReader.MoveToElement();
					}

					if (!wsKey.empty() && !wsValue.empty())
						Add(wsKey, wsValue);
				}
			}
		}
		void Add(const CWString& wsKey, const CWString& wsValue)
		{
			m_mFigures.insert(std::pair<CWString, CWString>(wsKey, wsValue));
		}

	private:

		std::map<CWString, CWString> m_mFigures;
	};
}

#endif //_XPS_XPSLIB_DOCUMENT_H