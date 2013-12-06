
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableCellProperties.h" 

namespace OOX
{
	namespace Logic
	{
		TableCellProperties::TableCellProperties()
		{

		}

		TableCellProperties::~TableCellProperties()
		{

		}	

		TableCellProperties::TableCellProperties(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableCellProperties& TableCellProperties::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableCellProperties::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			Width			=	element.element("tcW");
			tblBorders		=	element.element("tcBorders");
			VMerge			=	element.element("vMerge");
			Shading			=	element.element("shd");
			Background		=	element.element("shd").attribute("fill").value();
			GridSpan		=	element.element("gridSpan").attribute("val").value();
			CellMar			=	element.element("tcMar");
			VAlign			=	element.element("vAlign").attribute("val").value();
			CountVMerge		=	1; // TODO ???
		}

		const XML::XNode TableCellProperties::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX