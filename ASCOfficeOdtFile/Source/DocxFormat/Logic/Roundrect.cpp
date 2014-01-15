
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Roundrect.h"

namespace OOX
{
	namespace Logic
	{			
		Roundrect::Roundrect()
		{
	
		}

		Roundrect::~Roundrect()
		{
	
		}

		Roundrect::Roundrect (const XML::XNode& node)
		{
			fromXML(node);
		}

		const Roundrect& Roundrect::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Roundrect::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id					=	element.attribute("id").value();
			style				=	element.attribute("style").value();			
			arcsize				=	element.attribute("arcsize").value();
			Wrap				=	element.element("wrap");
			anchorlock			=	element.element("anchorlock");
		
			fillstyle			=	element;
			linestyle			=	element;
			shadow				=	element.element("shadow");
			
			textbox				=	element.element("textbox");
		}
		
		const XML::XNode Roundrect::toXML() const
		{			
			return XML::XElement(ns.v + "rect", XML::XAttribute("id", Id) +	XML::XAttribute("style", style.ToString()) + XML::XAttribute("arcsize", arcsize) + XML::Write(Wrap));
		}

	} // namespace Logic
} // namespace OOX
