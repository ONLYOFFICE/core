
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Bottom.h"


namespace OOX
{
	namespace Logic
	{

		Bottom::Bottom()
		{
		}


		Bottom::~Bottom()
		{
		}
	

		Bottom::Bottom(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Bottom& Bottom::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Bottom::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Value	= element.attribute("val").value();
			Space	= element.attribute("space").value();
			Size	= element.attribute("sz").value();
			Color	= element.attribute("color").value();
			ThemeColor = element.attribute("themeColor").value();
			ThemeShade = element.attribute("themeShade").value();			
		}


		const XML::XNode Bottom::toXML() const
		{
			return 
				XML::XElement(ns.w + "bottom",
					XML::XAttribute(ns.w + "val", Value) + 
					XML::XAttribute(ns.w + "space", Space) +
					XML::XAttribute(ns.w + "sz", Size) +
					XML::XAttribute(ns.w + "color", Color) +
					XML::XAttribute(ns.w + "themeColor", ThemeColor) +
					XML::XAttribute(ns.w + "themeShade", ThemeShade)
				);
		}
	} // namespace Logic
} // namespace OOX
