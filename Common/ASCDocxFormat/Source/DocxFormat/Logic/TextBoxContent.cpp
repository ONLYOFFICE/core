
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TextBoxContent.h"

namespace OOX
{
	namespace Logic
	{
		TextBoxContent::TextBoxContent()
		{

		}

		TextBoxContent::~TextBoxContent()
		{

		}	

		TextBoxContent::TextBoxContent(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TextBoxContent& TextBoxContent::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TextBoxContent::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				if (element.XName == "p" || element.XName == "tbl" || element.XName == "sdt" || element.XName == "bookmarkStart" || element.XName == "bookmarkEnd")
				{
					Logic::TextItem text(*i);
					items->push_back(text);
				}
			}
		}

		const XML::XNode TextBoxContent::toXML() const
		{			
			return XML::XElement();

			//return XML::XElement(ns.w + "txbxContent", XML::Write(items));
		}

	} // namespace Logic
} // namespace OOX
