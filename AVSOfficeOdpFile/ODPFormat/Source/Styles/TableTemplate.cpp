#include "TableTemplate.h"

namespace Odp
{
	namespace Styles
	{
		TableTemplate::TableTemplate()
		{
		}

		TableTemplate::~TableTemplate()
		{
		}

		TableTemplate::TableTemplate(const XML::XNode& node)
		{
			fromXML(node);
		}

		const TableTemplate& TableTemplate::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}

		void TableTemplate::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			StyleName = element.attribute("style-name").value();
			if(element.element("first-row").exist())
				FirstRow = element.element("first-row").attribute("style-name").value();
			if(element.element("last-row").exist())
				LastRow = element.element("last-row").attribute("style-name").value();
			if(element.element("first-column").exist())
				FirstColumn = element.element("first-column").attribute("style-name").value();
			if(element.element("last-column").exist())
				LastColumn = element.element("last-column").attribute("style-name").value();
			if(element.element("odd-rows").exist())
				OddRows = element.element("odd-rows").attribute("style-name").value();
			if(element.element("odd-columns").exist())
				OddColumns = element.element("odd-columns").attribute("style-name").value();
			if(element.element("body").exist())
				Body = element.element("body").attribute("style-name").value();
		}

		const XML::XNode TableTemplate::toXML() const
		{
			return XML::XElement(ns.table + "table-template",
				XML::XAttribute(ns.text + "style-name", StyleName) +
				XML::WriteIf(XML::XElement(ns.table + "first-row", XML::XAttribute(ns.text + "style-name", FirstRow)), FirstRow.is_init()) +
				XML::WriteIf(XML::XElement(ns.table + "last-row", XML::XAttribute(ns.text + "style-name", LastRow)), LastRow.is_init()) +
				XML::WriteIf(XML::XElement(ns.table + "first-column", XML::XAttribute(ns.text + "style-name", FirstColumn)), FirstColumn.is_init()) +
				XML::WriteIf(XML::XElement(ns.table + "last-column", XML::XAttribute(ns.text + "style-name", LastColumn)), LastColumn.is_init()) +
				XML::WriteIf(XML::XElement(ns.table + "odd-rows", XML::XAttribute(ns.text + "style-name", OddRows)), OddRows.is_init()) +
				XML::WriteIf(XML::XElement(ns.table + "odd-columns", XML::XAttribute(ns.text + "style-name", OddColumns)), OddColumns.is_init()) +
				XML::WriteIf(XML::XElement(ns.table + "body", XML::XAttribute(ns.text + "style-name", Body)), Body.is_init())
				);
		}
	} // namespace Styles;
} // namespace Odt