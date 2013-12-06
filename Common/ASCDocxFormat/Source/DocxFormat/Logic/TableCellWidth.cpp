
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TableCellWidth.h" 


namespace OOX
{
	namespace Logic
	{

		TableCellWidth::TableCellWidth()
		{
		}


		TableCellWidth::~TableCellWidth()
		{
		}
	

		TableCellWidth::TableCellWidth(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableCellWidth& TableCellWidth::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableCellWidth::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Width = element.attribute("w").value().ToString();
			Type	= element.attribute("type").value();
		}


		const XML::XNode TableCellWidth::toXML() const
		{
		return XML::XElement();
		}

	} // namespace Logic
} // namespace OOX