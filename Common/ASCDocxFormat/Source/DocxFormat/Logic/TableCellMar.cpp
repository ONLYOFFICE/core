
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableCellMar.h"


namespace OOX
{
	namespace Logic
	{

		TableCellMar::TableCellMar()
		{
		}


		TableCellMar::~TableCellMar()
		{
		}
	

		TableCellMar::TableCellMar(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableCellMar& TableCellMar::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableCellMar::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Top			=	element.element("top");
			Left		=	element.element("left");
			Bottom		=	element.element("bottom");
			Right		=	element.element("right");
		}

	
		const XML::XNode TableCellMar::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX