
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FontFace.h"


namespace Odt
{
	namespace Logic
	{

		FontFace::FontFace()
		{
		}


		FontFace::~FontFace()
		{
		}


		FontFace::FontFace(const XML::XNode& node)
		{
			fromXML(node);
		}


		const FontFace& FontFace::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void FontFace::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name		=	element.attribute("name").value();
			Family	=	element.attribute("font-family").value();
			Generic	= element.attribute("font-family-generic").value();
			Pitch		=	element.attribute("font-pitch").value();
		}


		const XML::XNode FontFace::toXML() const
		{
			return
				XML::XElement(ns.style + "font-face",
					XML::XAttribute(ns.style + "name", Name) +
					XML::XAttribute(ns.svg + "font-family", Family) +
					XML::XAttribute(ns.style + "font-family-generic", Generic) +
					XML::XAttribute(ns.style + "font-pitch", Pitch)
				);
		}

	} // namespace Logic
} // namespace Odt