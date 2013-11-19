#pragma once
#include <string>
#include "msxml.h"

namespace FB2Converter
{
	namespace XML
	{
		const std::wstring getAttr(MSXML2::IXMLDOMElementPtr element, const std::wstring& attrName);
	}
}