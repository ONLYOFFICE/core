
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableCell.h"


namespace Odt
{
	namespace Content
	{

		TableCell::TableCell()
		{
			m_hasNote = false;
			m_hasList = false;
		}


		TableCell::~TableCell()
		{
		}


		TableCell::TableCell(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableCell& TableCell::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableCell::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Style	= element.attribute("style-name").value();
			ColumnsSpanned	= element.attribute("number-columns-spanned").value();
			RowsSpanned	= element.attribute("number-rows-spanned").value();
			Type	= element.attribute("value-type").value();
		
			m_hasNote = false;
			m_hasList = false;
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				if (element.XName == "p" || element.XName == "h" || element.XName == "list" || element.XName == "table" || element.XName == "table-of-content" || element.XName == "index-title")
				{
					Text text(*i);
					m_hasList |= text.hasList();
					m_hasNote |= text.hasNote();
					Texts->push_back(text);
				}
			}
		}


		const XML::XNode TableCell::toXML() const
		{
			XML::XContainer container = 
				XML::XContainer(
					XML::XElement(ns.table + "table-cell",
						XML::XAttribute(ns.table + "style-name", Style) + 
						XML::XAttribute(ns.table + "number-rows-spanned", RowsSpanned) +
						XML::XAttribute(ns.table + "number-columns-spanned", ColumnsSpanned) +					
						XML::XAttribute(ns.office + "value-type", Type) +
						XML::Write(Texts)
					)
				);
			
			for (int count = ColumnsSpanned.get_value_or(1); count > 1; --count)
				container.Add(XML::XElement(ns.table + "covered-table-cell"));

			return container;
		}


		const bool TableCell::hasNote() const
		{
			return m_hasNote;
		}


		const bool TableCell::hasList() const
		{
			return m_hasList;
		}


		const bool TableCell::isFromHMerged() const
		{
			return ColumnsSpanned.is_init();
		}


		void TableCell::setNumCol(const int num)
		{
			m_numCol = num;
		}


		const int TableCell::getNumCol() const
		{
			return m_numCol;
		}

	} // namespace Content
} // namespace Odt