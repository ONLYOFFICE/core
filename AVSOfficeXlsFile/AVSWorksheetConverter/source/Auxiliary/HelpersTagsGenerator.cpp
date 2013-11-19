#include "stdafx.h"
#include "HelpersTagsGenerator.h"

namespace AUX
{;

HelpersTagsGenerator::HelpersTagsGenerator(MSXML2::IXMLDOMElementPtr document_root)
:	document_root_(document_root)
{
}

bool HelpersTagsGenerator::GenHelpers() 
{
	MSXML2::IXMLDOMDocumentPtr doc = document_root_->GetownerDocument();
	MSXML2::IXMLDOMElementPtr helpers_tag = XMLSTUFF::makeXMLNode(L"Helpers", document_root_);

	GenColumnsRefs(helpers_tag);
	GenIndexedColors(helpers_tag);

	return true;
}

void HelpersTagsGenerator::GenColumnsRefs(MSXML2::IXMLDOMElementPtr helpers_tag) 
{
	MSXML2::IXMLDOMDocumentPtr doc = document_root_->GetownerDocument();
	MSXML2::IXMLDOMElementPtr col_names_tag = XMLSTUFF::makeXMLNode(L"col_names", helpers_tag);
	for (int i = 0; i < 256; i++)
	{
		MSXML2::IXMLDOMElementPtr col_name_tag = doc->createElement(L"col_name");
		col_names_tag->appendChild(col_name_tag);
		col_name_tag->setAttribute(L"index", i);
		col_name_tag->setAttribute(L"ref", column2str(i).c_str());
	}

}

void HelpersTagsGenerator::GenIndexedColors(MSXML2::IXMLDOMElementPtr helpers_tag) 
{
	static std::wstring color_strings[64] = 
	{
		L"000000", L"FFFFFF", L"FF0000", L"00FF00", L"0000FF", L"FFFF00", L"FF00FF", L"00FFFF",
		L"000000", L"FFFFFF", L"FF0000", L"00FF00", L"0000FF", L"FFFF00", L"FF00FF", L"00FFFF",
		L"800000", L"008000", L"000080", L"808000", L"800080", L"008080", L"C0C0C0", L"808080",
		L"9999FF", L"993366", L"FFFFCC", L"CCFFFF", L"660066", L"FF8080", L"0066CC", L"CCCCFF",
		L"000080", L"FF00FF", L"FFFF00", L"00FFFF", L"800080", L"800000", L"008080", L"0000FF",
		L"00CCFF", L"CCFFFF", L"CCFFCC", L"FFFF99", L"99CCFF", L"FF99CC", L"CC99FF", L"FFCC99",
		L"3366FF", L"33CCCC", L"99CC00", L"FFCC00", L"FF9900", L"FF6600", L"666699", L"969696",
		L"003366", L"339966", L"003300", L"333300", L"993300", L"993366", L"333399", L"333333"
	};


	MSXML2::IXMLDOMDocumentPtr doc = document_root_->GetownerDocument();
	MSXML2::IXMLDOMElementPtr indexed_colors_tag = XMLSTUFF::makeXMLNode(L"indexedColors", helpers_tag);
	for (int i = 0; i < 64; i++)
	{
		MSXML2::IXMLDOMElementPtr rgb_color_tag = doc->createElement(L"rgbColor");
		indexed_colors_tag->appendChild(rgb_color_tag);
		rgb_color_tag->setAttribute(L"rgb", color_strings[i].c_str());
	}

}


} // namespace AUX

