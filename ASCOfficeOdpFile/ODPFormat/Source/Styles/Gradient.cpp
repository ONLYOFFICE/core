#include "Gradient.h"

namespace Odp
{
	namespace Styles
	{
		Gradient::Gradient()
		{
		}

		Gradient::~Gradient()
		{
		}

		Gradient::Gradient(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Gradient& Gradient::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Gradient::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Name = element.attribute("name").value();
			Style = element.attribute("style").value();
			DisplayName = element.attribute("display-name").value();
			StartColor = element.attribute("start-color").value();
			EndColor = element.attribute("end-color").value();
			StartIntensity = element.attribute("start-intensity").value().ToString();
			EndIntensity = element.attribute("end-intensity").value().ToString();
			Angle = element.attribute("angle").value();

			if(element.attribute("cx").exist())
				cx = element.attribute("cx").value().ToString();
			if(element.attribute("cy").exist())
				cy = element.attribute("cy").value().ToString();
			Border = element.attribute("border").value().ToString();
		}

		const XML::XNode Gradient::toXML() const
		{
			return
				XML::XElement(ns.draw + "gradient",
					XML::XAttribute(ns.draw + "name", Name) +
					XML::XAttribute(ns.draw + "display-name", DisplayName) +
					XML::XAttribute(ns.draw + "style", Style) +
					XML::XAttribute(ns.draw + "cx", cx) +
					XML::XAttribute(ns.draw + "cy", cy) +
					XML::XAttribute(ns.draw + "start-color", StartColor) +
					XML::XAttribute(ns.draw + "end-color", EndColor) +
					XML::XAttribute(ns.draw + "start-intensity", StartIntensity) +
					XML::XAttribute(ns.draw + "end-intensity", EndIntensity) +
					XML::XAttribute(ns.draw + "angle", Angle) +
					XML::XAttribute(ns.draw + "border", Border)
				);
		}
	} // namespace Styles;
} // namespace Odt