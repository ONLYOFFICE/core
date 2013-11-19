#include "TextBox.h"

namespace Odp
{
	namespace Content
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

		const TextBox& TextBox::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TextBox::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			XML::Fill(Items, element, "p", "list");
		}

		const XML::XNode TextBox::toXML() const
		{
			return 
				XML::XElement(ns.draw + "text-box", 
					XML::Write(Items)
				);
		}
	} // namespace Content
} // namespace Odp