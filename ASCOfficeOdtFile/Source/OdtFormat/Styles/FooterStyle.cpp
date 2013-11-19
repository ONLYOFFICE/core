
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FooterStyle.h"


namespace Odt
{
	namespace Styles
	{

		FooterStyle::FooterStyle()
		{
		}


		FooterStyle::~FooterStyle()
		{
		}


		FooterStyle::FooterStyle(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FooterStyle& FooterStyle::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FooterStyle::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			HeaderFooterProperties = element.element("header-footer-properties");
		}


		const XML::XNode FooterStyle::toXML() const
		{
			return XML::XElement(ns.style + "footer-style", XML::Write(HeaderFooterProperties));
		}

	} // namespace Styles
} // namespace Odt