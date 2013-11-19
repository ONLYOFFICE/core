#include "FillImage.h"

namespace Odp
{
	namespace Styles
	{
		FillImage::FillImage()
		{
		}

		FillImage::~FillImage()
		{
		}

		FillImage::FillImage(const XML::XNode& node)
		{
			fromXML(node);
		}

		const FillImage& FillImage::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void FillImage::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name = element.attribute("name").value();
			DisplayName = element.attribute("display-name").value();
			Href = element.attribute("href").value();
			Type = element.attribute("type").value();
			Show = element.attribute("show").value();
			Actuate = element.attribute("actuate").value();
		}

		const XML::XNode FillImage::toXML() const
		{
			return
				XML::XElement(ns.draw + "fill-image",
					XML::XAttribute(ns.draw + "name", Name) +
					XML::XAttribute(ns.draw + "display-name", DisplayName) +
					XML::XAttribute(ns.xlink + "href", Href) +
					XML::XAttribute(ns.xlink + "type", Type) +
					XML::XAttribute(ns.xlink + "show", Show) +
					XML::XAttribute(ns.xlink + "actuate", Actuate)
				);
		}
	} // namespace Styles;
} // namespace Odt