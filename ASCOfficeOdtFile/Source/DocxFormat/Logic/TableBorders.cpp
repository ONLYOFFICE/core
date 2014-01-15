
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableBorders.h"

namespace OOX
{
	namespace Logic
	{
		TableBorders::TableBorders()
		{

		}

		TableBorders::~TableBorders()
		{

		}	

		TableBorders::TableBorders(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableBorders& TableBorders::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableBorders::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			top			=	element.element("top");
			bottom		=	element.element("bottom");
			left		=	element.element("left");
			right		=	element.element("right");
			insideV		=	element.element("insideV");
			insideH		=	element.element("insideH");
		}

		const XML::XNode TableBorders::toXML() const
		{
			return XML::XElement(ns.w + "tblBorders", 
				XML::Write(top) +
				XML::Write(bottom) +
				XML::Write(left) +
				XML::Write(right) +
				XML::Write(insideV) +
				XML::Write(insideH));
		}

	} // namespace Logic
} // namespace OOX