
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "HeaderStyle.h"


namespace Odt
{
	namespace Styles
	{

		HeaderStyle::HeaderStyle()
		{
		}


		HeaderStyle::~HeaderStyle()
		{
		}


		HeaderStyle::HeaderStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const HeaderStyle& HeaderStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void HeaderStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			HeaderFooterProperties = element.element("header-footer-properties");
		}


		const XML::XNode HeaderStyle::toXML() const
		{
			return XML::XElement(ns.style + "header-style", XML::Write(HeaderFooterProperties));
		}

	} // namespace Styles
} // namespace Odt