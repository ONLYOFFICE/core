
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "HeaderFooterProperties.h"


namespace Odt
{
	namespace Styles
	{

		HeaderFooterProperties::HeaderFooterProperties()
		{
		}


		HeaderFooterProperties::~HeaderFooterProperties()
		{
		}


		HeaderFooterProperties::HeaderFooterProperties(const XML::XNode& node)
		{
			fromXML(node);
		}


		const HeaderFooterProperties& HeaderFooterProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void HeaderFooterProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Height			= element.attribute("height").value();
			MinHeight		= element.attribute("min-height").value();
			MarginRight		= element.attribute("margin-top").value();
			MarginLeft		= element.attribute("margin-left").value();
			MarginRight		= element.attribute("margin-right").value(); 
			MarginBottom	= element.attribute("margin-bottom").value();
			DynamicSpacing	= element.attribute("dynamic-spacing").value();
		}


		const XML::XNode HeaderFooterProperties::toXML() const
		{
			return 
				XML::XElement(ns.style + "header-footer-properties",
					XML::XAttribute(ns.svg + "height", Height) +
					XML::XAttribute(ns.fo + "min-height", MinHeight) +
					XML::XAttribute(ns.fo + "margin-top", MarginTop) +
					XML::XAttribute(ns.fo + "margin-left", MarginLeft) +
					XML::XAttribute(ns.fo + "margin-right", MarginRight) +
					XML::XAttribute(ns.fo + "margin-bottom", MarginBottom) +
					XML::XAttribute(ns.fo + "dynamic-spacing", DynamicSpacing)
				);
		}
	} // namespace Styles
} // namespace Odt