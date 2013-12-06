
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../FootNote.h"

namespace OOX
{
	FootNote::Note::Note()
	{
	}

	FootNote::Note::~Note()
	{
	}
	
	FootNote::Note::Note(const XML::XNode& node)
	{
		fromXML(node);
	}

	const FootNote::Note& FootNote::Note::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}

	void FootNote::Note::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		Type	= element.attribute("type").value();
		Id		= element.attribute("id").value();
		XML::Fill(Items, element , "p", "tbl");	
	}

	const XML::XNode FootNote::Note::toXML() const
	{
		return XML::XElement();

		//return
		//	XML::XElement(ns.w + "footnote",
		//		XML::XAttribute(ns.w + "type", Type) +
		//		XML::XAttribute(ns.w + "id", Id) +
		//		XML::Write(Items)
		//	);
	}

	void FootNote::Note::push_back(const OOX::Logic::Paragraph& paragraph)
	{
		Items->push_back(paragraph);
	}

} // namespace OOX