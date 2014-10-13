#include "stdafx.h"
#include "Converter.h"
#include <Document/Document.h>


const bool Converter::convert(Document& doc, MSXML2::IXMLDOMDocumentPtr xslt)
{
	return VARIANT_TRUE == doc.getDoc()->loadXML(doc.getDoc()->transformNode(xslt));
}

