#include "Origin.h"

namespace PPTX
{
	namespace nsViewProps
	{
		Origin::Origin()
		{
		}

		Origin::~Origin()
		{
		}

		Origin::Origin(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Origin& Origin::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Origin::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			x = element.attribute("x").value();
			y = element.attribute("y").value();
		}

		const XML::XNode Origin::toXML() const
		{
			return	XML::XElement(ns.p + "origin",
				XML::XAttribute("x", x) +
				XML::XAttribute("y", y)
			);
		}
	} // namespace nsViewProps
} // namespace PPTX