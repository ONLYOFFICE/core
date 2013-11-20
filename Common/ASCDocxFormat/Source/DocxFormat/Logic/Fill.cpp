
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Fill.h"


namespace OOX
{
	namespace Logic
	{
			
		Fill::Fill()
		{
		}


		Fill::~Fill()
		{
		}


		Fill::Fill(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Fill& Fill::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Fill::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Color	= element.attribute("color2").value();
			Colors	= element.attribute("colors").value();
			Method	= element.attribute("method").value();
			Rotate	= element.attribute("rotate").value();
			Focus	= element.attribute("focus").value();
			Type	= element.attribute("type").value();
		}

		
		const XML::XNode Fill::toXML() const
		{			
			return
				XML::XElement(ns.v + "fill",
					XML::XAttribute("color2", Color) +
					XML::XAttribute("rotate", Rotate) +
					XML::XAttribute("colors", Colors) +
					XML::XAttribute("method", Method) +
					XML::XAttribute("focus", Focus) +
					XML::XAttribute("type", Type)
				);
		}

	} // namespace Logic
} // namespace OOX