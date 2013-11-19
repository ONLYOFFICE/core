
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TableCellItem.h"
#include "CoveredTableCell.h"
#include "TableCell.h"


namespace Odt
{
	namespace Content
	{

		TableCellItem::TableCellItem()
		{
		}


		TableCellItem::~TableCellItem()
		{
		}


		TableCellItem::TableCellItem(const TableCell& tableCell)
			: m_item(new TableCell(tableCell))
		{
		}


		TableCellItem::TableCellItem(const CoveredTableCell& coveredTableCell)
			: m_item(new CoveredTableCell(coveredTableCell))
		{
		}


		TableCellItem::TableCellItem(const XML::XNode& node)
		{
			fromXML(node);
		}


		const TableCellItem& TableCellItem::operator =(const XML::XNode& node)
		{
			fromXML(node);
			return *this;
		}


		void TableCellItem::fromXML(const XML::XNode& node)
		{
			const XML::XElement element(node);
			if (element->XName == "table-cell")
				m_item.reset(new TableCell(node));
			else if (element->XName == "covered-table-cell")
				m_item.reset(new CoveredTableCell(node));

		}


		const XML::XNode TableCellItem::toXML() const
		{
			return m_item->toXML();
		}


		const bool TableCellItem::hasNote() const
		{
			return m_item->hasNote();
		}


		const bool TableCellItem::hasList() const
		{
			return m_item->hasList();
		}


		void TableCellItem::setNumCol(const int num)
		{
			m_item->setNumCol(num);
		}


		const int TableCellItem::getNumCol() const
		{
			return m_item->getNumCol();
		}


		const int TableCellItem::getCountHMerge() const
		{
			if (is<TableCell>())
				return as<TableCell>().ColumnsSpanned.get_value_or(1);
			else
				return as<CoveredTableCell>().CountHMerge;
		}

	} // namespace Content
} // namespace