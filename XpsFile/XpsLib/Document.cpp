#include "Document.h"
#include "StaticResources.h"

#include "../../Common/DocxFormat/Source/XML/xmlutils.h"

namespace XPS
{
	CDocument::CDocument(CFontManager* pFontManager)
	{		
		m_pFontManager = pFontManager;
		m_mPages.clear();
	}
	CDocument::~CDocument()
	{		
		Close();
	}
	bool CDocument::ReadFromPath(const std::wstring& wsPath)
	{
		Close();

		m_wsPath = wsPath;

		XmlUtils::CXmlLiteReader oReader;

		std::wstring wsRelsPath = NormalizePath(wsPath + L"_rels/.rels");

		clock_t oBeginTime = clock();

		if (!oReader.FromFile(wsRelsPath))
			return false;

		clock_t oEndTime = clock();
		double dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
		printf("%S %fseconds\n", wsRelsPath.c_str(), dElapsedSecs);

		if (!oReader.ReadNextNode())
			return false;

		std::wstring wsName = oReader.GetName();
		if (L"Relationships" != wsName)
			return false;

		std::wstring wsFile;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetName();
			if (L"Relationship" == wsName)
			{
				std::wstring wsAttr;
				ReadAttribute(oReader, L"Type", wsAttr);

				if (L"http://schemas.microsoft.com/xps/2005/06/fixedrepresentation" == wsAttr)
				{
					ReadAttribute(oReader, L"Target", wsFile);
					break;
				}
			}
		}

		if (wsFile.empty())
			return false;

		oReader.Clear();

		oBeginTime = clock();

		if (!oReader.FromFile(wsPath + wsFile))
			return false;

		oEndTime = clock();
		dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
		printf("%S %fseconds\n", (wsPath + wsFile).c_str(), dElapsedSecs);

		if (!oReader.ReadNextNode())
			return false;

		wsName = oReader.GetName();
		if (L"FixedDocumentSequence" != wsName)
			return false;

		wsFile.clear();
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetName();
			if (L"DocumentReference" == wsName)
			{
				ReadAttribute(oReader, L"Source", wsFile);
				break;
			}
		}

		if (wsFile.empty())
			return false;

		oReader.Clear();

		oBeginTime = clock();

		if (!oReader.FromFile(m_wsPath + wsFile))
			return false;

		oEndTime = clock();
		dElapsedSecs = double(oEndTime - oBeginTime) / CLOCKS_PER_SEC;
		printf("%S %fseconds\n", (m_wsPath + wsFile).c_str(), dElapsedSecs);

		if (!oReader.ReadNextNode())
			return false;

		wsName = oReader.GetName();
		if (L"FixedDocument" != wsName)
			return false;

		std::wstring wsFilePath = GetPath(m_wsPath + wsFile);
		std::wstring wsPagePath;
		std::wstring wsSource;

		int nIndex = 0;
		while (oReader.ReadNextNode())
		{
			wsName = oReader.GetName();

			if (L"PageContent" == wsName)
			{
				ReadAttribute(oReader, L"Source", wsSource);
				if ('/' == wsSource[0])
					wsPagePath = m_wsPath + wsSource;
				else
					wsPagePath = wsFilePath + wsSource;

				m_mPages.insert(std::pair<int, XPS::Page*>(nIndex++, new XPS::Page(wsPagePath, wsPath, &m_oFontList, m_pFontManager, this)));
			}

		}

		return true;		
	}
	int  CDocument::GetPageCount()const
	{
		return (int)m_mPages.size();
	}
	void CDocument::GetPageSize(int nPageIndex, int& nW, int& nH)
	{
		std::map<int, XPS::Page*>::const_iterator oIter = m_mPages.find(nPageIndex);
		if (oIter != m_mPages.end())
			oIter->second->GetSize(nW, nH);
	}
	void CDocument::DrawPage(int nPageIndex, IRenderer* pRenderer, bool* pbBreak)
	{
		std::map<int, XPS::Page*>::const_iterator oIter = m_mPages.find(nPageIndex);
		if (oIter != m_mPages.end())
			oIter->second->Draw(pRenderer, pbBreak);
	}
	void CDocument::Close()
	{
		for (std::map<int, XPS::Page*>::iterator oIter = m_mPages.begin(); oIter != m_mPages.end(); oIter++)
		{
			if (oIter->second)
				delete oIter->second;
		}
		m_mPages.clear();
		m_oFontList.Clear();

		for (std::map<std::wstring, CStaticResource*>::iterator oIter = m_mStaticResources.begin(); oIter != m_mStaticResources.end(); oIter++)
		{
			if (oIter->second)
				delete oIter->second;
		}
		m_mStaticResources.clear();
	}
	CStaticResource* CDocument::GetStaticResource(const wchar_t* wsPath)
	{
		for (auto oIt : m_mStaticResources)
		{
			if (oIt.first == wsPath)
				return oIt.second;
		}

		CStaticResource* pStaticResource = new CStaticResource(wsPath);
		m_mStaticResources.insert(std::pair<std::wstring, CStaticResource*>(wsPath, pStaticResource));
		return pStaticResource;
	}
}