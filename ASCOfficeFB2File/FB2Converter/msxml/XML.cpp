#include "XML.h"

namespace FB2Converter
{
	namespace XML
	{
		const std::wstring getAttr(MSXML2::IXMLDOMElementPtr element, const std::wstring& attrName)
		{
			MSXML2::IXMLDOMNamedNodeMapPtr attrs = element->Getattributes();
			while( MSXML2::IXMLDOMAttributePtr attr = attrs->nextNode() )
				if ((std::wstring)attr->GetbaseName() == attrName)
					return attr->GetnodeValue().bstrVal;
			return L"";
		}
	}
}