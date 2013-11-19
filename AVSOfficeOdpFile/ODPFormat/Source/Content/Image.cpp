#include "Image.h"

namespace Odp
{
	namespace Content
	{
		Image::Image()
		{
		}

		Image::~Image()
		{
		}

		Image::Image(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Image& Image::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Image::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			href = element.attribute("href").value();
			type = element.attribute("type").value();
			show = element.attribute("show").value();
			actuate = element.attribute("actuate").value();

			XML::Fill(Items, element, "p", "list");
		}

		const XML::XNode Image::toXML() const
		{
			return 
				XML::XElement(ns.draw + "image", 
					XML::XAttribute(ns.xlink + "href", href) +
					XML::XAttribute(ns.xlink + "type", type) +
					XML::XAttribute(ns.xlink + "show", show) + 
					XML::XAttribute(ns.xlink + "actuate", actuate) +
					XML::Write(Items)
				);
		}
	} // namespace Content
} // namespace Odp