#include "PageLayout.h"

namespace Odp
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
			Name = element.attribute("name").value();
			Properties = element.element("page-layout-properties");
		}

		const XML::XNode PageLayout::toXML() const
		{
			return
				XML::XElement(ns.style + "page-layout",
					XML::XAttribute(ns.style + "name", Name) +
					XML::Write(Properties)
				);
		}
	} // namespace Styles;
} // namespace Odt