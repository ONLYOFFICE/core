
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"


namespace OOX
{

	Numbering::LevelOverride::LevelOverride()
	{
	}


	Numbering::LevelOverride::~LevelOverride()
	{
	}


	Numbering::LevelOverride::LevelOverride(const XML::XNode& node)
	{
		fromXML(node);
	}


	const Numbering::LevelOverride& Numbering::LevelOverride::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}


	void Numbering::LevelOverride::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);
		Ilvl = element.attribute("ilvl").value();
	    StartOverride = element.element("startOverride").attribute("val").value();
        Level = element.element("lvl");
	}


	const XML::XNode Numbering::LevelOverride::toXML() const
	{
		return
			XML::XElement(ns.w + "lvlOverride",
				XML::XAttribute(ns.w + "ilvl", Ilvl) +
				XML::Write(ns.w + "startOverride", ns.w + "val", StartOverride) +
				XML::Write(Level)
			);
	}

} // namespace OOX