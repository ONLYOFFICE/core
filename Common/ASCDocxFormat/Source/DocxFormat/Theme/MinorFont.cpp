
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "MinorFont.h"


namespace OOX
{
	namespace Theme
	{

		MinorFont::MinorFont()
		{
		}


		MinorFont::~MinorFont()
		{
		}
	

		MinorFont::MinorFont(const XML::XNode& node)
		{
			fromXML(node);
		}


		const MinorFont& MinorFont::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void MinorFont::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			latin = element.element("latin").attribute("typeface").value();
			ea		= element.element("ea").attribute("typeface").value();
			cs		= element.element("cs").attribute("typeface").value();
			XML::Fill(fonts, element, "font");
		}


		const XML::XNode MinorFont::toXML() const
		{
			return 
				XML::XElement(ns.a + "minorFont",
					XML::XElement(ns.a + "latin", XML::XAttribute("typeface", latin)) +
					XML::XElement(ns.a + "ea", XML::XAttribute("typeface", ea)) +
					XML::XElement(ns.a + "cs", XML::XAttribute("typeface", cs)) +
					XML::Write(fonts)
				);
		}

	} // namespace Theme
} // namespace OOX