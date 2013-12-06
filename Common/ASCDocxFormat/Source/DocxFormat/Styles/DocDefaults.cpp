
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Styles.h"
#include "XML.h"


namespace OOX
{

	Styles::DocDefaults::DocDefaults()
	{
	}


	Styles::DocDefaults::~DocDefaults()
	{
	}
	

	Styles::DocDefaults::DocDefaults(const XML::XNode& node)
	{
		fromXML(node);
	}


	const Styles::DocDefaults& Styles::DocDefaults::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void Styles::DocDefaults::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		ParagraphProperty = element.element("pPrDefault").element("pPr");
		RunProperty				= element.element("rPrDefault").element("rPr");
	}


	const XML::XNode Styles::DocDefaults::toXML() const
	{
		return XML::XElement();
	}

} // namespace OOX