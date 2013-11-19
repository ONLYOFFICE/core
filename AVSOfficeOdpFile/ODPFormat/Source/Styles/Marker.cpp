#include "Marker.h"

namespace Odp
{
	namespace Styles
	{

		Marker::Marker()
		{
		}

		Marker::~Marker()
		{
		}

		Marker::Marker(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Marker& Marker::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Marker::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
			DisplayName = element.attribute("display-name").value();
			ViewBox = element.attribute("viewBox").value();
			d = element.attribute("d").value();
		}

		const XML::XNode Marker::toXML() const
		{
			return 
				XML::XElement(ns.draw + "marker", 
					XML::XAttribute(ns.draw + "name", Name) +
					XML::XAttribute(ns.draw + "display-name", DisplayName) +
					XML::XAttribute(ns.svg + "viewBox", ViewBox) +
					XML::XAttribute(ns.svg + "d", d)
				);
		}
	} // namespace Styles
} // namespace Odp