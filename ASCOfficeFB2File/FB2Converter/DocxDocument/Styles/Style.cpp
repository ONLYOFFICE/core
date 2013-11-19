#include "Styles.h"
#include <string>

namespace FB2Converter
{
	static const CString s_filename = _T("\\word\\styles.xml");

	Styles::Styles(const CString filepath)
		:m_filepath(filepath)
	{
		m_styleElems = m_pXmlDoc->createElement("w:styles");
	}

	Styles::~Styles()
	{
	}


	bool Styles::Read()
	{
		if (m_pXmlDoc->load(static_cast<_bstr_t>(m_filepath + s_filename))  == VARIANT_TRUE)
			return true;
		return false;
	}


	bool Styles::Write(const CString filepath)
	{
		AddStyles();

		MSXML2::IXMLDOMElementPtr styles_node = m_pXmlDoc->selectSingleNode(L"w:styles");
		MSXML2::IXMLDOMNodeListPtr styles  = m_styleElems->selectNodes( L"w:style" );
        while( MSXML2::IXMLDOMNodePtr style = styles->nextNode() )
			styles_node->appendChild(style);

		m_pXmlDoc->removeChild(styles_node);
		m_pXmlDoc->appendChild(styles_node);

		if (m_pXmlDoc->save(static_cast<_bstr_t>(filepath + s_filename)) == S_OK)
			return true;
		return false;
	}


	void Styles::AddStyles()
	{
		AddTitleStyle(L"");
		AddEpigraphStyle(L"EpigraphStyle");
		AddStrongStyle(L"StrongStyle");
		AddEmphasisStyle(L"EmphasisStyle");
		AddPoemLineStyle(L"PoemLineStyle");
		AddParagraphStyle(L"ParagraphStyle");
	}


	void Styles::AddTitleStyle(const std::wstring& styleName)
	{

	}
	

	void Styles::AddEpigraphStyle(const std::wstring& styleName)
	{
		MSXML2::IXMLDOMElementPtr style_node = m_pXmlDoc->createElement(L"w:style");
		style_node->setAttribute(L"w:type", (const _variant_t &)L"paragraph");
		style_node->setAttribute(L"w:styleId", (const _variant_t &)styleName.c_str());

		MSXML2::IXMLDOMElementPtr name_node = m_pXmlDoc->createElement(L"w:name");
		name_node->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());
		MSXML2::IXMLDOMElementPtr pPr_node = m_pXmlDoc->createElement(L"w:pPr");
		MSXML2::IXMLDOMElementPtr ind_node = m_pXmlDoc->createElement(L"w:ind");
		ind_node->setAttribute(L"w:left", (const _variant_t &)L"4200");
		MSXML2::IXMLDOMElementPtr jc_node = m_pXmlDoc->createElement(L"w:jc");
		jc_node->setAttribute(L"w:val", (const _variant_t &)L"right");

		MSXML2::IXMLDOMElementPtr rPr_node = m_pXmlDoc->createElement(L"w:rPr");
		MSXML2::IXMLDOMElementPtr i_node = m_pXmlDoc->createElement(L"w:i");

		pPr_node->appendChild(ind_node);	
		pPr_node->appendChild(jc_node);
		rPr_node->appendChild(i_node);

		style_node->appendChild(name_node);
		style_node->appendChild(pPr_node);
		style_node->appendChild(rPr_node);
		m_styleElems->appendChild(style_node);
	}
	

	void Styles::AddStrongStyle(const std::wstring& styleName)
	{
		MSXML2::IXMLDOMElementPtr style_node = m_pXmlDoc->createElement(L"w:style");
		style_node->setAttribute(L"w:type", (const _variant_t &)L"paragraph");
		style_node->setAttribute(L"w:styleId", (const _variant_t &)styleName.c_str());

		MSXML2::IXMLDOMElementPtr name_node = m_pXmlDoc->createElement(L"w:name");
		name_node->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());

		MSXML2::IXMLDOMElementPtr pPr_node = m_pXmlDoc->createElement(L"w:pPr");
		MSXML2::IXMLDOMElementPtr jc_node = m_pXmlDoc->createElement(L"w:jc");
		jc_node->setAttribute(L"w:val", (const _variant_t &)L"center");

		MSXML2::IXMLDOMElementPtr rPr_node = m_pXmlDoc->createElement(L"w:rPr");
		MSXML2::IXMLDOMElementPtr bold_node = m_pXmlDoc->createElement(L"w:b");

		rPr_node->appendChild(bold_node);
		pPr_node->appendChild(jc_node);

		style_node->appendChild(name_node);
		style_node->appendChild(pPr_node);
		style_node->appendChild(rPr_node);
		m_styleElems->appendChild(style_node);
	}
	
	
	void Styles::AddEmphasisStyle(const std::wstring& styleName)
	{
		MSXML2::IXMLDOMElementPtr style_node = m_pXmlDoc->createElement(L"w:style");
		style_node->setAttribute(L"w:type", (const _variant_t &)L"character");
		style_node->setAttribute(L"w:styleId", (const _variant_t &)styleName.c_str());

		MSXML2::IXMLDOMElementPtr name_node = m_pXmlDoc->createElement(L"w:name");
		name_node->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());
		MSXML2::IXMLDOMElementPtr rPr_node = m_pXmlDoc->createElement(L"w:rPr");
		MSXML2::IXMLDOMElementPtr italic_node = m_pXmlDoc->createElement(L"w:i");

		rPr_node->appendChild(italic_node);
		style_node->appendChild(name_node);
		style_node->appendChild(rPr_node);
		m_styleElems->appendChild(style_node);
	}


	void Styles::AddPoemLineStyle(const std::wstring& styleName)
	{
		MSXML2::IXMLDOMElementPtr style_node = m_pXmlDoc->createElement(L"w:style");
		style_node->setAttribute(L"w:type", (const _variant_t &)L"paragraph");
		style_node->setAttribute(L"w:styleId", (const _variant_t &)styleName.c_str());

		MSXML2::IXMLDOMElementPtr name_node = m_pXmlDoc->createElement(L"w:name");
		name_node->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());
		MSXML2::IXMLDOMElementPtr pPr_node = m_pXmlDoc->createElement(L"w:pPr");
		MSXML2::IXMLDOMElementPtr indent_node = m_pXmlDoc->createElement(L"w:ind");
		indent_node->setAttribute(L"w:left", (const _variant_t &)"3500");
		MSXML2::IXMLDOMElementPtr spacing_node = m_pXmlDoc->createElement(L"w:spacing");
		spacing_node->setAttribute(L"w:after", (const _variant_t &)"0");
		spacing_node->setAttribute(L"w:line", (const _variant_t &)"240");
		spacing_node->setAttribute(L"w:lineRule", (const _variant_t &)"auto");

		pPr_node->appendChild(spacing_node);
		pPr_node->appendChild(indent_node);
		style_node->appendChild(name_node);
		style_node->appendChild(pPr_node);
		m_styleElems->appendChild(style_node);
	}

	void Styles::AddParagraphStyle(const std::wstring& styleName)
	{
		MSXML2::IXMLDOMElementPtr style_node = m_pXmlDoc->createElement(L"w:style");
		style_node->setAttribute(L"w:type", (const _variant_t &)L"paragraph");
		style_node->setAttribute(L"w:styleId", (const _variant_t &)styleName.c_str());

		MSXML2::IXMLDOMElementPtr name_node = m_pXmlDoc->createElement(L"w:name");
		name_node->setAttribute(L"w:val", (const _variant_t &)styleName.c_str());
		MSXML2::IXMLDOMElementPtr pPr_node = m_pXmlDoc->createElement(L"w:pPr");
		MSXML2::IXMLDOMElementPtr indent_node = m_pXmlDoc->createElement(L"w:ind");
		indent_node->setAttribute(L"w:firstLine", (const _variant_t &)"500");
		MSXML2::IXMLDOMElementPtr jc_node = m_pXmlDoc->createElement(L"w:jc");
		jc_node->setAttribute(L"w:val", (const _variant_t &)"both");

		pPr_node->appendChild(indent_node);
		pPr_node->appendChild(jc_node);
		style_node->appendChild(name_node);
		style_node->appendChild(pPr_node);
		m_styleElems->appendChild(style_node);
	}

}