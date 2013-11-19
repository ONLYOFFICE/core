#include "Placeholder.h"

namespace Odp
{
	namespace Styles
	{

		Placeholder::Placeholder()
		{
		}

		Placeholder::~Placeholder()
		{
		}

		Placeholder::Placeholder(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Placeholder& Placeholder::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Placeholder::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			object = element.attribute("object").value();
			x = element.attribute("x").value().ToString();
			y = element.attribute("y").value().ToString();
			width = element.attribute("width").value().ToString();
			height = element.attribute("height").value().ToString();
		}

		const XML::XNode Placeholder::toXML() const
		{
			return 
				XML::XElement(ns.presentation + "placeholder", 
					XML::XAttribute(ns.presentation + "object", object) +
					XML::XAttribute(ns.svg + "x", x) +
					XML::XAttribute(ns.svg + "y", y) +
					XML::XAttribute(ns.svg + "width", width) +
					XML::XAttribute(ns.svg + "height", height)
				);
		}
	} // namespace Styles
} // namespace Odp