#pragma once


class Document;

class Converter
{
public:
	virtual const bool convertToX(Document& doc) = 0;
	virtual const bool convertFromX(Document& doc, const _bstr_t& xml_options, const _bstr_t& file_name) = 0;

protected:
	const bool convert(Document& doc, MSXML2::IXMLDOMDocumentPtr xslt);

};

