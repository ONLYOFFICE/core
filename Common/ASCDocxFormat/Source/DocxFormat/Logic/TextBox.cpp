
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextBox.h"

namespace OOX
{
	namespace Logic
	{			
		TextBox::TextBox()
		{

		}

		TextBox::~TextBox()
		{

		}	

		TextBox::TextBox(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TextBox& TextBox::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TextBox::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			content	=	element.element("txbxContent");
			style	=	element.attribute("style").value();
			inset	=	element.attribute("inset").value();
		}

		const XML::XNode TextBox::toXML() const
		{			
			return XML::XElement(ns.v + "textbox", XML::XAttribute("style", style) + XML::XAttribute("inset", inset) + XML::Write(content));
		}
	} // namespace Logic
} // namespace OOX
