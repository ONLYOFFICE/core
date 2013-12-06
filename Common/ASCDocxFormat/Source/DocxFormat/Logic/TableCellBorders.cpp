
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableCellBorders.h" 

namespace OOX
{
	namespace Logic
	{
		TableCellBorders::TableCellBorders()
		{

		}

		TableCellBorders::~TableCellBorders()
		{

		}	

		TableCellBorders::TableCellBorders(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableCellBorders& TableCellBorders::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableCellBorders::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			top			=	element.element("top");
			bottom		=	element.element("bottom");
			left		=	element.element("left");
			right		=	element.element("right");
			insideH		=	element.element("insideH");
			insideV		=	element.element("insideV");
		}

		const XML::XNode TableCellBorders::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX