
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Image.h"


namespace Odt
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
			Href		= element.attribute("href").value();
			Type		= element.attribute("type").value();
			Show		=	element.attribute("show").value();
			Actuate =	element.attribute("actuate").value();				
		}


		const XML::XNode Image::toXML() const
		{
			return
				XML::XElement(ns.draw + "image",
					XML::XAttribute(ns.xlink + "href", Href) + 
					XML::XAttribute(ns.xlink + "type", Type) +
					XML::XAttribute(ns.xlink + "show", Show) +
					XML::XAttribute(ns.xlink + "actuate", Actuate)
				);
		}
		
	} // namespace Content
} // namespace Odt