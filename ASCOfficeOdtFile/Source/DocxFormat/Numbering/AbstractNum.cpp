
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "./../Numbering.h"
#include "Exception/log_runtime_error.h"

namespace OOX
{
	Numbering::AbstractNum::AbstractNum()
	{

	}

	Numbering::AbstractNum::~AbstractNum()
	{

	}

	Numbering::AbstractNum::AbstractNum(const XML::XNode& node)
	{
		fromXML(node);
	}

	const Numbering::AbstractNum& Numbering::AbstractNum::operator =(const XML::XNode& node)
	{
		fromXML(node);
		return *this;
	}

	void Numbering::AbstractNum::fromXML(const XML::XNode& node)
	{
		const XML::XElement element(node);

		Id						=	element.attribute("abstractNumId").value();
		Nsid					=	element.element("nsid").attribute("val").value();
		MultiLevelType			=	element.element("multiLevelType").attribute("val").value();
		Tmpl					=	element.element("tmpl").attribute("val").value();

		if (element.element("numStyleLink").exist())
		{
			if (element.element("numStyleLink").attribute("val").exist())
				numStyleLink	=	element.element("numStyleLink").attribute("val").value();
		}

		XML::Fill(Levels, element, "lvl");
	}

	const XML::XNode Numbering::AbstractNum::toXML() const
	{
		return 
			XML::XElement(ns.w + "abstractNum", 
			XML::XAttribute(ns.w + "abstractNumId", Id) +
			XML::Write(ns.w + "multiLevelType", ns.w + "val", MultiLevelType) +
			XML::Write(Levels) + 
			XML::Write(ns.w + "tmpl", ns.w + "val", Tmpl) +
			XML::Write(ns.w + "nsid", ns.w + "val", Nsid));
	}

	const Numbering::Level Numbering::AbstractNum::getLevel(const int numLevel) const
	{
		BOOST_FOREACH(const Level& level, *Levels)
		{
			if (level.Ilvl == numLevel)
				return level;
		}
		throw log_runtime_error("bad abstractNum");
	}

} // namespace OOX