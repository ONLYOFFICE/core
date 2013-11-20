#pragma once
#ifndef XML_EXTENSION_WRITE_IF_INCLUDE_H_
#define XML_EXTENSION_WRITE_IF_INCLUDE_H_

#include "nullable.h"
#include "property.h"
#include "nullable_property.h"
#include "./../XName.h"
#include "./../XNode.h"
#include "./../XElement.h"
#include "./../XAttribute.h"


namespace XML
{
	const XML::XNode WriteIf(const XName& xname, const bool write);
	const XML::XNode WriteIf(const XName& xname, const nullable<bool> write);
	const XML::XNode WriteIf(const XName& xname, const property<bool> write);
	const XML::XNode WriteIf(const XName& xname, const nullable_property<bool> write);

	const XML::XNode WriteIf(const XML::XElement& element, const bool write);
	const XML::XNode WriteIf(const XML::XElement& element, const nullable<bool> write);
	const XML::XNode WriteIf(const XML::XElement& element, const property<bool> write);
	const XML::XNode WriteIf(const XML::XElement& element, const nullable_property<bool> write);

	const XML::XNode WriteIf(const XML::XNode& node, const bool write);
	const XML::XNode WriteIf(const XML::XNode& node, const nullable<bool> write);
	const XML::XNode WriteIf(const XML::XNode& node, const property<bool> write);
	const XML::XNode WriteIf(const XML::XNode& node, const nullable_property<bool> write);

	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const bool write);
	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const nullable<bool> write);
	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const property<bool> write);
	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const nullable_property<bool> write);
} // namespace XML

#endif // XML_EXTENSION_WRITE_IF_INCLUDE_H_