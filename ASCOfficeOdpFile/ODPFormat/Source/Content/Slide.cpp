#include "Slide.h"

namespace Odp
{
	namespace Content
	{
		Slide::Slide()
		{
		}

		Slide::~Slide()
		{
		}

		Slide::Slide(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Slide& Slide::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Slide::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
			StyleName = element.attribute("style-name").value();
			MasterPageName = element.attribute("master-page-name").value();
			Id = element.attribute("id").value();

			Forms = element.element("forms");
			XML::Fill(Elements, element, "frame", "custom-shape", "g", "line", "rect");
			Animation = element.element("par");
		}

		const XML::XNode Slide::toXML() const
		{
			return 
				XML::XElement(ns.draw + "page", 
					XML::XAttribute(ns.draw + "name", Name) +
					XML::XAttribute(ns.draw + "style-name", StyleName) +
					XML::XAttribute(ns.draw + "master-page-name", MasterPageName) +
					XML::XAttribute(ns.draw + "id", Id) +
					XML::Write(Forms) +
					XML::Write(Elements) +
					XML::Write(Animation)
				);
		}
	} // namespace Content
} // namespace Odp