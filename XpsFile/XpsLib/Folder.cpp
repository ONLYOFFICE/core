#include "Folder.h"
#include "../../Common/DocxFormat/Source/XML/xmlutils.h"

namespace XPS
{
	Folder::Folder(CFontManager* pFontManager)
	{		
		m_pFontManager = pFontManager;
		m_mPages.clear();
	}
	Folder::~Folder()
	{		
		Close();
	}
	bool Folder::ReadFromPath(const std::wstring& wsPath)
	{
		Close();

		m_wsPath = wsPath;

		XmlUtils::CXmlNode oNode;
		XmlUtils::CXmlNodes arrNodes;
		XmlUtils::CXmlNode oSubnode;
		
		std::wstring wsRelsPath = NormalizePath(wsPath + L"_rels/.rels");
		if (!oNode.FromXmlFile(wsRelsPath.c_str()))
			return false;

		// Checking root node
		if (L"Relationships" != oNode.GetName())
			return false;

		if (!oNode.GetNodes(L"Relationship", arrNodes))
			return false;

		std::wstring wsFile;
		for (int nIndex = 0, nCount = arrNodes.GetCount(); nIndex < nCount; nIndex++)
		{
			arrNodes.GetAt(nIndex, oNode);

			if (L"http://schemas.microsoft.com/xps/2005/06/fixedrepresentation" == oNode.GetAttribute(L"Type"))
			{
				wsFile = oNode.GetAttribute(L"Target");
				break;
			}

			if (nIndex == nCount - 1)
				return false;
		}

		if (!oNode.FromXmlFile((wsPath + wsFile).c_str()))
			return false;;

		// Checking root node
		if (L"FixedDocumentSequence" != oNode.GetName())
			return false;

		if (!oNode.GetNode(L"DocumentReference", oSubnode))
			return false;

		wsFile = oSubnode.GetAttribute(L"Source");

		if (!oNode.FromXmlFile((m_wsPath + wsFile).c_str()))
			return false;

		if (L"FixedDocument" != oNode.GetName())
			return false;

		if (!oNode.GetNodes(L"PageContent", arrNodes))
			return false;

		std::wstring wsFilePath = GetPath(m_wsPath + wsFile);
		std::wstring wsPagePath;
		std::wstring wsSource;

		for (int nIndex = 0, nCount = arrNodes.GetCount(); nIndex < nCount; nIndex++)
		{
			arrNodes.GetAt(nIndex, oNode);
			wsSource = oNode.GetAttribute(L"Source");
			if('/' == wsSource[0])
				wsPagePath = m_wsPath + wsSource;
			else
				wsPagePath = wsFilePath + wsSource;

			m_mPages.insert(std::pair<int, XPS::Page*>(nIndex, new XPS::Page(wsPagePath, wsPath, &m_oFontList, m_pFontManager)));
		}

		return true;
	}
	int  Folder::GetPageCount()const
	{
		return (int)m_mPages.size();
	}
	void Folder::GetPageSize(int nPageIndex, int& nW, int& nH)
	{
		std::map<int, XPS::Page*>::const_iterator oIter = m_mPages.find(nPageIndex);
		if (oIter != m_mPages.end())
			oIter->second->GetSize(nW, nH);
	}
	void Folder::DrawPage(int nPageIndex, IRenderer* pRenderer, bool* pbBreak)
	{
		std::map<int, XPS::Page*>::const_iterator oIter = m_mPages.find(nPageIndex);
		if (oIter != m_mPages.end())
			oIter->second->Draw(pRenderer, pbBreak);
	}
	void Folder::Close()
	{
		for (std::map<int, XPS::Page*>::iterator oIter = m_mPages.begin(); oIter != m_mPages.end(); oIter++)
		{
			if (oIter->second)
				delete oIter->second;
		}
		m_mPages.clear();
		m_oFontList.Clear();
	}
}