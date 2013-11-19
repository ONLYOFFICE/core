
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableColumn.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
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
				Width		= element.attribute("column-width").value();
				RelWidth	= element.attribute("rel-column-width").value();
			}


			const XML::XNode TableColumn::toXML() const
			{
				return 
					XML::XElement(ns.style + "table-column-properties",
						XML::XAttribute(ns.style + "column-width", Width) +
						XML::XAttribute(ns.style + "rel-column-width", RelWidth)
					);
			}


			const bool TableColumn::isSimple() const
			{
				if (Width.is_init() || RelWidth.is_init())
					return false;
				return true;
			}


			// TODO устаревшая
/*			const TableColumn TableColumn::merge(const TableColumn& rhs) const
			{
				TableColumn properties = *this;
				properties.Width = rhs.Width;
				properties.RelWidth = rhs.RelWidth;
				return properties;
			}*/



			const TableColumn TableColumn::merge(const TableColumn& lhs, const TableColumn& rhs)
			{
				TableColumn properties;
				properties.Width	= ::merge(lhs.Width, rhs.Width);
				properties.RelWidth = ::merge(lhs.RelWidth, rhs.RelWidth);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt