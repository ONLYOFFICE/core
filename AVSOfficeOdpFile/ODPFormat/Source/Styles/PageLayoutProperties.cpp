#include "PageLayoutProperties.h"

namespace Odp
{
	namespace Styles
	{
		PageLayoutProperties::PageLayoutProperties()
		{
		}

		PageLayoutProperties::~PageLayoutProperties()
		{
		}

		PageLayoutProperties::PageLayoutProperties(const XML::XNode& node)
		{
			fromXML(node);
		}

		const PageLayoutProperties& PageLayoutProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void PageLayoutProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			margin_top = element.attribute("margin-top").value().ToString();
			margin_left = element.attribute("margin-left").value().ToString();
			margin_right = element.attribute("margin-right").value().ToString();
			margin_bottom = element.attribute("margin-bottom").value().ToString();
			page_height = element.attribute("page-height").value().ToString();
			page_width = element.attribute("page-width").value().ToString();
			print = element.attribute("print-orientation").value();
		}

		const XML::XNode PageLayoutProperties::toXML() const
		{
			return
				XML::XElement(ns.style + "page-layout-properties",
					XML::XAttribute(ns.fo + "margin-top", margin_top) +
					XML::XAttribute(ns.fo + "margin-bottom", margin_bottom) +
					XML::XAttribute(ns.fo + "margin-left", margin_left) +
					XML::XAttribute(ns.fo + "margin-right", margin_right) +
					XML::XAttribute(ns.fo + "page-width", page_width) +
					XML::XAttribute(ns.fo + "page-height", page_height) +
					XML::XAttribute(ns.style + "print-orientation", print)
				);
		}

		const bool PageLayoutProperties::isSimple() const
		{
			return true;
		}
	} // namespace Styles
} // namespace Odp