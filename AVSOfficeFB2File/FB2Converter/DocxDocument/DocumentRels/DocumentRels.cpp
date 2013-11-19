#include "DocumentRels.h"
#include <string>
#include "./../../msxml/XML.h"

namespace FB2Converter
{
	static const CString s_filename = _T("\\word\\_rels\\document.xml.rels");
	static const int c_nStart_rId = 9;

	DocumentRels::DocumentRels(const CString filepath)
		:m_filepath(filepath)
	{
		m_relsElems = m_pXmlDoc->createElement("w:body");	
	}

	DocumentRels::~DocumentRels()
	{
	}


	bool DocumentRels::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool DocumentRels::Write(const CString filepath)
	{
		MSXML2::IXMLDOMElementPtr rels_node = m_pXmlDoc->selectSingleNode(L"Relationships");
		MSXML2::IXMLDOMNodeListPtr rels  = m_relsElems->selectNodes( L"Relationship" );
        while( MSXML2::IXMLDOMNodePtr rel = rels->nextNode() )
			rels_node->appendChild(rel);		

		m_pXmlDoc->removeChild(rels_node);
		m_pXmlDoc->appendChild(rels_node);		

		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)		
			return true;
		return false;
	}

	void DocumentRels::AddImage(const std::wstring& name)
	{
		MSXML2::IXMLDOMElementPtr rel = m_pXmlDoc->createNode((const _variant_t &)1, L"Relationship", L"http://schemas.openxmlformats.org/package/2006/relationships");

		int id = c_nStart_rId + m_relsElems->GetchildNodes()->Getlength() + 1;
		char buf[10];
		std::string rId = "rId" + std::string(::itoa(id, buf, 10));

		rel->setAttribute(L"Id", (const _variant_t &)rId.c_str());
		rel->setAttribute(L"Type", (const _variant_t &)"http://schemas.openxmlformats.org/officeDocument/2006/relationships/image");
		rel->setAttribute(L"Target", (const _variant_t &)((std::wstring)(L"media/" + name)).c_str());

		m_relsElems->appendChild(rel);
	}


	const std::string DocumentRels::AddHyperlink(const std::wstring& href)
	{
		MSXML2::IXMLDOMElementPtr rel = m_pXmlDoc->createNode((const _variant_t &)1, L"Relationship", L"http://schemas.openxmlformats.org/package/2006/relationships");

		int id = c_nStart_rId + m_relsElems->GetchildNodes()->Getlength() + 1;
		char buf[10];
		std::string rId = "rId" + std::string(::itoa(id, buf, 10));

		rel->setAttribute(L"Id", (const _variant_t &)rId.c_str());
		rel->setAttribute(L"Type", (const _variant_t &)"http://schemas.openxmlformats.org/officeDocument/2006/relationships/hyperlink");
		rel->setAttribute(L"Target", (const _variant_t &)href.c_str());
		rel->setAttribute(L"TargetMode", (const _variant_t &)"External");

		m_relsElems->appendChild(rel);

		return rId;
	}


	const std::wstring DocumentRels::GetRId(const std::wstring& name)
	{		
		MSXML2::IXMLDOMNodeListPtr rels  = m_relsElems->selectNodes( L"Relationship" );
        while( MSXML2::IXMLDOMNodePtr rel = rels->nextNode() )
		{
			std::wstring target = XML::getAttr(rel, L"Target");
			if (target.find(name, 0) != std::wstring::npos)
				return (std::wstring)XML::getAttr(rel, L"Id");
		}
		std::wstring rId;
		return rId;
	}


}