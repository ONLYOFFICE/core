
// auto inserted precompiled begin
#include "precompiled_xml.h"
// auto inserted precompiled end

#include "WriteIf.h"


namespace XML
{

	const XML::XNode WriteIf(const XName& xname, const bool write)
	{
		if (write)
			return XML::XElement(xname);
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XName& xname, const nullable__<bool> write)
	{
		if (write.get_value_or(false))
			return XML::XElement(xname);
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XName& xname, const property<bool> write)
	{
		if (write)
			return XML::XElement(xname);
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XName& xname, const nullable_property<bool> write)
	{
		if (write.get_value_or(false))
			return XML::XElement(xname);
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XElement& element, const bool write)
	{
		if (write)
			return element;
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XElement& element, const nullable__<bool> write)
	{
		if (write.get_value_or(false))
			return element;
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XElement& element, const property<bool> write)
	{
		if (write)
			return element;
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XElement& element, const nullable_property<bool> write)
	{
		if (write.get_value_or(false))
			return element;
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XNode& node, const bool write)
	{
		if (write)
			return node;
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XNode& node, const nullable__<bool> write)
	{
		if (write.get_value_or(write))
			return node;
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XNode& node, const property<bool> write)
	{
		if (write)
			return node;
		return XML::XNode();
	}


	const XML::XNode WriteIf(const XML::XNode& node, const nullable_property<bool> write)
	{
		if (write.get_value_or(false))
			return node;
		return XML::XNode();
	}


	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const bool write)
	{
		if (write)
			return attribute;
		return XML::XAttribute();
	}


	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const nullable__<bool> write)
	{
		if (write.get_value_or(false))
			return attribute;
		return XML::XAttribute();
	}


	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const property<bool> write)
	{
		if (write)
			return attribute;
		return XML::XAttribute();
	}


	const XML::XAttribute WriteIf(const XML::XAttribute& attribute, const nullable_property<bool> write)
	{
		if (write.get_value_or(false))
			return attribute;
		return XML::XAttribute();
	}

} // namespace XML