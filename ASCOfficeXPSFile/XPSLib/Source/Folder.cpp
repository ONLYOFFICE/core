#include "stdafx.h"
#include "Folder.h"

namespace XPS
{
	Folder::Folder()
		:	m_bIsClosed(false)
		,	m_lDrawingPages(0)
	{
		InitializeCriticalSection(&m_oFontCS);
		m_fontMap = new CAtlMap<CString, bool>();
	}

	Folder::~Folder()
	{
		DeleteCriticalSection(&m_oFontCS);
		for(std::map<int, XPS::Page*>::iterator iter = m_pages.begin(); iter != m_pages.end(); iter++)
		{
			delete iter->second;
		}
		m_pages.erase(m_pages.begin(), m_pages.end());
	}

	void Folder::ReadFromPath(const CString& Path)
	{
		if(m_bIsClosed)
			return;

		m_lDrawingPages++;

		m_path = Path;
		m_pages.erase(m_pages.begin(), m_pages.end());
		m_fontMap->RemoveAll();
		
		//XmlUtils::CXmlNodes	nodes;
		//XmlUtils::CXmlNode	node;
		
		BOOL result = m_node.FromXmlFile(Path + _T("_rels/.rels"));
		if(!result)
		{
			m_lDrawingPages--;
			return;
		}

		m_node.ReadRootNode(_T("Relationships"));
		result = m_node.GetNodes(_T("Relationship"), m_nodes);
		if(!result)
		{
			m_lDrawingPages--;
			return;
		}

		CString file;// = _T("FixedDocSeq.fdseq");
		for(int i = 0; i < m_nodes.GetCount(); i++)
		{
			m_nodes.GetAt(i, m_node);
			if(m_node.GetAttribute(_T("Type")) == _T("http://schemas.microsoft.com/xps/2005/06/fixedrepresentation"))
			{
				file = m_node.GetAttribute(_T("Target"));
				break;
			}
			if(i == m_nodes.GetCount() - 1)
			{
				m_lDrawingPages--;
				return;
			}
		}

		result = m_node.FromXmlFile(m_path + file);
		if(!result)
		{
			m_lDrawingPages--;
			return;
		}

		m_node.ReadRootNode(_T("FixedDocumentSequence"));
		result = m_node.GetNode(_T("DocumentReference"), m_subnode);
		if(!result)
		{
			m_lDrawingPages--;
			return;
		}

		file = m_subnode.GetAttribute(_T("Source"));
		result = m_node.FromXmlFile(m_path + file);
		if(!result)
		{
			m_lDrawingPages--;
			return;
		}

		m_node.ReadRootNode(_T("FixedDocument"));
		result = m_node.GetNodes(_T("PageContent"), m_nodes);
		if(!result)
		{
			m_lDrawingPages--;
			return;
		}

		CString str1 = GetPath(m_path + file);
		CString str2;
		CString source;
		for(int i = 0; i < m_nodes.GetCount(); i++)
		{
			m_nodes.GetAt(i, m_node);
			source = m_node.GetAttribute(_T("Source"));
			if(source[0] == _T('/'))
				str2 = m_path + source;
			else
				str2 = str1 + source;
			m_pages.insert(std::make_pair<int, XPS::Page*>(i, new XPS::Page(str2, m_path, m_fontMap, &m_oFontCS)));
		}
		m_lDrawingPages--;
	}

	int Folder::GetPageCount()const
	{
		return (int)m_pages.size();
	}

	void Folder::GetPageSize(int Number, int& w, int& h)
	{
		if(m_bIsClosed)
			return;

		m_lDrawingPages++;
		std::map<int, XPS::Page*>::const_iterator iter = m_pages.find(Number);
		if(iter != m_pages.end())
			iter->second->GetSize(w, h);
		m_lDrawingPages--;
	}

	void Folder::DrawPage(int Number, ASCGraphics::IASCRenderer* pRenderer, BOOL* pBreak)
	{
		if(m_bIsClosed)
			return;

		m_lDrawingPages++;
		std::map<int, XPS::Page*>::const_iterator iter = m_pages.find(Number);
		if(iter != m_pages.end())
			iter->second->Draw(pRenderer, pBreak);
		m_lDrawingPages--;
	}

	void Folder::Close()
	{
		m_bIsClosed = true;
		while(m_lDrawingPages > 0){Sleep(100);}
	}
} // namespace XPS