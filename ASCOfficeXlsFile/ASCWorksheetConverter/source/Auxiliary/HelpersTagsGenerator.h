#pragma once

namespace AUX
{;

class HelpersTagsGenerator
{
public:
	HelpersTagsGenerator(MSXML2::IXMLDOMElementPtr document_root);

	bool GenHelpers();

private:
	void GenColumnsRefs(MSXML2::IXMLDOMElementPtr helpers_tag);
	void GenIndexedColors(MSXML2::IXMLDOMElementPtr helpers_tag);

	MSXML2::IXMLDOMElementPtr document_root_;
};



} // namespace AUX



