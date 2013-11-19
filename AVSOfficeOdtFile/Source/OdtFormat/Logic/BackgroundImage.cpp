
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BackgroundImage.h"


namespace Odt
{
	namespace Logic
	{

		BackgroundImage::BackgroundImage()
		{
		}


		BackgroundImage::~BackgroundImage()
		{
		}


		BackgroundImage::BackgroundImage(const XML::XNode& node)
		{
			fromXML(node);
		}


		const BackgroundImage& BackgroundImage::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void BackgroundImage::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Href		= element.attribute("href").value();
			Type		= element.attribute("type").value();
			Actuate =	element.attribute("actuate").value();				
		}


		const XML::XNode BackgroundImage::toXML() const
		{
			return
				XML::XElement(ns.draw + "background-image",
					XML::XAttribute(ns.xlink + "href", Href) + 
					XML::XAttribute(ns.xlink + "type", Type) +
					XML::XAttribute(ns.xlink + "actuate", Actuate)
				);
		}
		
	} // namespace Logic
} // namespace Odt