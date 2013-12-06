
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../EndNote.h"


namespace OOX
{
	EndNote::Note::Note()
	{
	}

	EndNote::Note::~Note()
	{
	}	

	EndNote::Note::Note(const XML::XNode& node)
	{
		fromXML(node);
	}

	const EndNote::Note& EndNote::Note::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}

	void EndNote::Note::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		Type	= element.attribute("type").value();
		Id		= element.attribute("id").value();
		XML::Fill(Items, element , "p", "tbl");			
	}


	const XML::XNode EndNote::Note::toXML() const
	{			
		return
			XML::XElement(ns.w + "endnote",
			  XML::XAttribute(ns.w + "type", Type) +
				XML::XAttribute(ns.w + "id", Id) +
				XML::Write(Items)
			);
	}


	void EndNote::Note::push_back(const OOX::Logic::Paragraph& paragraph)
	{
		Items->push_back(paragraph);
	}

} // namespace OOX