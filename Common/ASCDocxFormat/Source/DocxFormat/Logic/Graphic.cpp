
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Graphic.h"


namespace OOX
{
	namespace Logic
	{

		Graphic::Graphic()
		{
		}


		Graphic::~Graphic()
		{
		}
	

		Graphic::Graphic(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Graphic& Graphic::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Graphic::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Uri = element.element("graphicData").attribute("uri").value();
			Pic = element.element("graphicData").element("pic");
		}


		const XML::XNode Graphic::toXML() const
		{
			return
				XML::XElement(ns.a + "graphic", 
					XML::XElement(ns.a + "graphicData", 
						XML::XAttribute("uri", Uri) + 
						XML::Write(Pic)
					)
				);
		}

	} // namespace Logic
} // namespace OOX