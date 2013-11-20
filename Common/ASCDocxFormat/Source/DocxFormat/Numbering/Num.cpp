
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"


namespace OOX
{

	Numbering::Num::Num()
	{
	}


	Numbering::Num::~Num()
	{
	}


	Numbering::Num::Num(const XML::XNode& node)
	{
		fromXML(node);
	}


	const Numbering::Num& Numbering::Num::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void Numbering::Num::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		NumId					= element.attribute("numId").value();
		AbstractNumId = element.element("abstractNumId").attribute("val").value();
		XML::Fill(LevelOverrides, element, "lvlOverride");
	}


	const XML::XNode Numbering::Num::toXML() const
	{
		return
			XML::XElement(ns.w + "num",
				XML::XAttribute(ns.w + "numId", NumId) +
				XML::XElement(ns.w + "abstractNumId", XML::XAttribute(ns.w + "val", AbstractNumId)) +
				XML::Write(LevelOverrides)
			);
	}

} // namespace OOX