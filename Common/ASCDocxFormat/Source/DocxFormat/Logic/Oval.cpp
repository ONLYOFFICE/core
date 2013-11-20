
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Oval.h"

namespace OOX
{
	namespace Logic
	{
		Oval::Oval()
		{

		}

		Oval::~Oval()
		{

		}

		Oval::Oval(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Oval& Oval::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Oval::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Id					=	element.attribute("id").value();
			style				=	element.attribute("style").value();			
			Wrap				=	element.element("wrap");
			anchorlock			=	element.element("anchorlock");

			fillstyle			=	element;
			linestyle			=	element;
			shadow				=	element.element("shadow");
			
			textbox				=	element.element("textbox");
		}

		const XML::XNode Oval::toXML() const
		{			
			return XML::XElement(ns.v + "oval", XML::XAttribute("id", Id) +	XML::XAttribute("style", style.ToString()) + XML::Write(Wrap));
		}

	} // namespace Logic
} // namespace OOX
