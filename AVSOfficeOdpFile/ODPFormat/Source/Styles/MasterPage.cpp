#include "MasterPage.h"

namespace Odp
{
	namespace Styles
	{
		MasterPage::MasterPage()
		{
		}

		MasterPage::~MasterPage()
		{
		}

		MasterPage::MasterPage(const XML::XNode& node)
		{
			fromXML(node);
		}

		const MasterPage& MasterPage::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void MasterPage::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
			Layout = element.attribute("page-layout-name").value();
			StyleName = element.attribute("style-name").value();

			XML::Fill(Elements, element, "frame", "custom-shape", "g", "line", "rect");
		}

		const XML::XNode MasterPage::toXML() const
		{
			return
				XML::XElement(ns.style + "master-page",
					XML::XAttribute(ns.style + "name", Name) +
					XML::XAttribute(ns.style + "page-layout-name", Layout) +
					XML::XAttribute(ns.draw + "style-name", StyleName) +
					XML::Write(Elements)
				);
		}
	} // namespace Styles;
} // namespace Odt