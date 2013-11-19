
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PageLayout.h"


namespace Odt
{
	namespace Styles
	{

		PageLayout::PageLayout()
		{
		}


		PageLayout::~PageLayout()
		{
		}


		PageLayout::PageLayout(const XML::XNode& node)
		{
			fromXML(node);
		}


		const PageLayout& PageLayout::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void PageLayout::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name				= element.attribute("name").value();
			Properties	= element.element("page-layout-properties");
			HeaderStyle	=	element.element("header-style");
			FooterStyle	=	element.element("footer-style");
		}


		const XML::XNode PageLayout::toXML() const
		{
			return
				XML::XElement(ns.style + "page-layout",
					XML::XAttribute(ns.style + "name", Name) +
					XML::Write(Properties) +
					XML::Write(HeaderStyle) +
					XML::Write(FooterStyle)
				);
		}

	} // namespace Styles
} // namespace Odt