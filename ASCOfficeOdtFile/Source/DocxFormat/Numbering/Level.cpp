
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"


namespace OOX
{

	Numbering::Level::Level()
	{
	}


	Numbering::Level::~Level()
	{
	}


	Numbering::Level::Level(const XML::XNode& node)
	{
		fromXML(node);
	}


	const Numbering::Level& Numbering::Level::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void Numbering::Level::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		Ilvl		= element.attribute("ilvl").value();
		Tplc		= element.attribute("tplc").value();
		Start		= element.element("start").attribute("val").value();	
		NumFmt	= element.element("numFmt");
		Suffix	= element.element("suff").attribute("val").value();
		Text		=	element.element("lvlText").attribute("val").value();
		Align		=	element.element("lvlJc").attribute("val").value();
		ParagraphProperty	= element.element("pPr");
		RunProperty				=	element.element("rPr");
		Tentative					= element.attribute("tentative").value();
	}


	const XML::XNode Numbering::Level::toXML() const
	{
		return
			XML::XElement(ns.w + "lvl",
				XML::XAttribute(ns.w + "ilvl", Ilvl) +
				XML::XAttribute(ns.w + "tplc", Tplc) + 
				XML::XAttribute(ns.w + "tentative", Tentative) +
				XML::Write(ns.w + "start", ns.w + "val", Start) +
				XML::Write(NumFmt) +
				XML::Write(ns.w + "suff", ns.w + "val", Suffix) +
				XML::Write(ns.w + "lvlText", ns.w + "val", Text)+					
				XML::Write(ParagraphProperty) +
				XML::Write(RunProperty) + 
				XML::Write(ns.w + "lvlJc", ns.w + "val", Align)
			);
	}

} // namespace OOX