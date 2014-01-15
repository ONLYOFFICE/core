
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Rect.h"

namespace OOX
{
	namespace Logic
	{
		Rect::Rect()
		{

		}

		Rect::~Rect()
		{

		}

		Rect::Rect(const XML::XNode& node)
		{
			fromXML(node);
		}

		const Rect& Rect::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void Rect::fromXML(const XML::XNode& node)
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

		const XML::XNode Rect::toXML() const
		{			
			return XML::XElement(ns.v + "rect", XML::XAttribute("id", Id) + XML::XAttribute("style", style.ToString()) + XML::Write(Wrap));
		}

	} // namespace Logic
} // namespace OOX
