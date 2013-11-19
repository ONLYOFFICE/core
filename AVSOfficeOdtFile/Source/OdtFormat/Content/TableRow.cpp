
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableRow.h"
#include "TableCell.h"
#include "CoveredTableCell.h"
#include "Exception/log_runtime_error.h"


namespace Odt
{
	namespace Content
	{

		TableRow::TableRow()
		{
			m_hasNote = false;
			m_hasList = false;
		}


		TableRow::~TableRow()
		{
		}


		TableRow::TableRow(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableRow& TableRow::operator = (const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableRow::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);

			StyleName = element.attribute("style-name").value();
			DefaultCellStyleName = element.attribute("default-cell-style-name").value();

			m_hasNote = false;
			m_hasList = false;
			int numCol = 0;
			for (XML::const_element_iterator i = element.Elements.begin(); i != element.Elements.end(); ++i)
			{
				TableCellItem cell(*i);
				m_hasList |= cell.hasList();
				m_hasNote |= cell.hasNote();
				cell.setNumCol(numCol);
				Cells->push_back(cell);

				if (cell.is<TableCell>())
				{
					int count = cell.as<TableCell>().ColumnsSpanned.get_value_or(1);
					while(count-- > 1)
					{
						numCol++;
						i++;
					}
				}
				numCol++;
			}
		}


		const XML::XNode TableRow::toXML() const
		{
			return
				XML::XElement(ns.table + "table-row",
					XML::XAttribute(ns.table + "style-name", StyleName) +
					XML::XAttribute(ns.table + "default-cell-style-name", DefaultCellStyleName) +
					XML::Write(Cells)
				);
		}


		const bool TableRow::hasNote() const
		{
			return m_hasNote;
		}


		const bool TableRow::hasList() const
		{
			return m_hasList;
		}


		const TableCellItem TableRow::getCellByNumCol(const int num) const
		{
			BOOST_FOREACH(const TableCellItem& cell, *Cells)
			{
				if (cell.getNumCol() == num)
					return cell;
			}
			throw log_runtime_error("bad num column");
		}

	} // namespace Content
} // namespace Odt