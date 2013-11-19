#pragma once

#include <Converter/Converter.h>

namespace HTML
{;


class HTMLConverter : public Converter
{
public:
	HTMLConverter();

	virtual const bool convertToX(Document& doc);
	virtual const bool convertFromX(Document& doc, const _bstr_t& xml_options, const _bstr_t& file_name);

private:
	MSXML2::IXMLDOMDocument3Ptr PrepareX2H_XSLT();

// private:
// 	MSXML2::IXSLTemplatePtr xslt_x2h;
};

} // namespace XLS

