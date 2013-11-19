
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableRow.h"


namespace Odt
{
	namespace Logic
	{
		namespace Property
		{

			TableRow::TableRow()
			{
			}


			TableRow::~TableRow()
			{
			}


			TableRow::TableRow(const XML::XNode& node)
			{
				fromXML(node);
			}


			const TableRow& TableRow::operator =(const XML::XNode& node)
			{
				fromXML(node);
				return *this;
			}


			void TableRow::fromXML(const XML::XNode& node)
			{
				const XML::XElement element(node);
				Height			= element.attribute("row-height").value();
				MinHeight		= element.attribute("min-row-height").value();
				KeepTogether	= element.attribute(ns.style + "keep-together").value();
				FoKeepTogether	= element.attribute(ns.fo + "keep-together").value();
			}


			const XML::XNode TableRow::toXML() const
			{
				return 
					XML::XElement(ns.style + "table-row-properties",
						XML::XAttribute(ns.style + "row-height", Height) +
						XML::XAttribute(ns.style + "min-row-height", MinHeight) +
						XML::XAttribute(ns.style + "keep-together", KeepTogether) +
						XML::XAttribute(ns.fo + "keep-together", FoKeepTogether)
					);
			}


			const bool TableRow::isSimple() const
			{
				if (Height.is_init() || MinHeight.is_init() || KeepTogether.is_init() || FoKeepTogether.is_init())
					return false;
				return true;
			}


			const TableRow TableRow::merge(const TableRow& prev, const TableRow& current)
			{
				TableRow properties;
				properties.Height			= ::merge(prev.Height, current.Height);
				properties.MinHeight		= ::merge(prev.MinHeight, current.MinHeight);
				properties.KeepTogether		= ::merge(prev.KeepTogether, current.KeepTogether);
				properties.FoKeepTogether	= ::merge(prev.FoKeepTogether, current.FoKeepTogether);
				return properties;
			}

		} // namespace Property
	} // namespace Logic
} // namespace Odt