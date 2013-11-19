
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableColumn.h"


namespace Odt
{
	namespace Content
	{

		TableColumn::TableColumn()
		{
		}


		TableColumn::~TableColumn()
		{
		}


		TableColumn::TableColumn(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableColumn& TableColumn::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableColumn::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Style			= element.attribute("style-name").value();
			Repeated	= element.attribute("number-columns-repeated").value();
		}


		const XML::XNode TableColumn::toXML() const
		{
			return 
				XML::XElement(ns.table + "table-column",
					XML::XAttribute(ns.table + "style-name", Style) +
					XML::XAttribute(ns.table + "number-columns-repeated", Repeated)
				);
		}

	} // namespace Content
} // namespace Odt