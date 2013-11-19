
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "MajorFont.h"


namespace OOX
{
	namespace Theme
	{

		MajorFont::MajorFont()
		{
		}


		MajorFont::~MajorFont()
		{
		}
	

		MajorFont::MajorFont(const XML::XNode& node)
		{
			fromXML(node);
		}


		const MajorFont& MajorFont::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void MajorFont::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			latin	=	element.element("latin").attribute("typeface").value();
			ea		=	element.element("ea").attribute("typeface").value();
			cs		=	element.element("cs").attribute("typeface").value();
			XML::Fill(fonts, element, "font");
		}


		const XML::XNode MajorFont::toXML() const
		{
			return
				XML::XElement(ns.a + "majorFont",
					XML::XElement(ns.a + "latin", XML::XAttribute("typeface", latin)) +
					XML::XElement(ns.a + "ea", XML::XAttribute("typeface", ea)) +
					XML::XElement(ns.a + "cs", XML::XAttribute("typeface", cs)) +
					XML::Write(fonts)
				);
		}

	} // namespace Theme
} // namespace OOX