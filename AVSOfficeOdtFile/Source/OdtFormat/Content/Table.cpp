
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Table.h"
#include "TableCell.h"
#include "CoveredTableCell.h"


namespace Odt
{
	namespace Content
	{

		Table::Table()
		{
			m_hasNote = false;
			m_hasList = false;
		}


		Table::~Table()
		{
		}


		Table::Table(const XML::XNode& node)
		{
			fromXML(node);
		}


		const Table& Table::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void Table::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			Name		= element.attribute("name").value();
			Style		= element.attribute("style-name").value();
			TemplateName = element.attribute("template-name").value();
			UseBandingColumnsStyles = element.attribute("use-banding-columns-styles").value();
			UseBandingRowsStyles	= element.attribute("use-banding-rows-styles").value();
			UseFirstColumnStyles	= element.attribute("use-first-column-styles").value();
			UseFirstRowStyles			= element.attribute("use-first-row-styles").value();
			UseLastColumnStyles		= element.attribute("use-last-column-styles").value();
			UseLastRowStyles			= element.attribute("use-last-row-styles").value();
			IsSubTable = element.attribute("is-sub-table").value();	

			Header	= element.element("table-header-rows");

			XML::Fill(Columns, element, "table-column");

			m_hasNote = false;
			m_hasList = false;
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				const XML::XElement element(*i);
				if (element.XName == "table-row")
				{
					TableRow row(*i);
					m_hasList |= row.hasList();
					m_hasNote |= row.hasNote();
					Rows->push_back(row);
				}
			}

			SetCountsHMerged();
		}


		const XML::XNode Table::toXML() const
		{
			return 
				XML::XElement(ns.table + "table",
					XML::XAttribute(ns.table + "template-name", TemplateName) +
					XML::XAttribute(ns.table + "use-banding-columns-styles", UseBandingColumnsStyles) +
					XML::XAttribute(ns.table + "use-banding-rows-styles", UseBandingRowsStyles) +
					XML::XAttribute(ns.table + "use-first-column-styles", UseFirstColumnStyles) +
					XML::XAttribute(ns.table + "use-first-row-styles", UseFirstRowStyles) +
					XML::XAttribute(ns.table + "use-last-column-styles", UseLastColumnStyles) +
					XML::XAttribute(ns.table + "use-last-row-styles", UseLastRowStyles) +
					XML::XAttribute(ns.table + "name", Name) +
					XML::XAttribute(ns.table + "style-name", Style) +
					XML::XAttribute(ns.table + "is-sub-table", IsSubTable) +
					XML::Write(Header) +
					XML::Write(Columns) + 
					XML::Write(Rows)
				);
		}


		const bool Table::hasNote() const
		{
			return m_hasNote;
		}


		const bool Table::hasList() const
		{
			return m_hasList;
		}


		void Table::SetCountsHMerged()
		{		
			for (size_t j = 0; j != Rows->size(); ++j)
			{
				std::vector<TableCellItem> row;
				for (size_t i = 0; i !=  Rows->at(j).Cells->size(); ++i)
				{
					if (Rows->at(j).Cells->at(i).is<CoveredTableCell>())
					{
						const int count = Rows->at(j-1).getCellByNumCol(Rows->at(j).Cells->at(i).getNumCol()).getCountHMerge();
						Rows->at(j).Cells->at(i).as<CoveredTableCell>().CountHMerge = count;	
						row.push_back(Rows->at(j).Cells->at(i));
						i += count - 1;
					}
					else
					{
						row.push_back(Rows->at(j).Cells->at(i));
					}
				}
				Rows->at(j).Cells = row;
			}
		}
	} // namespace Content
} // namespace Odt