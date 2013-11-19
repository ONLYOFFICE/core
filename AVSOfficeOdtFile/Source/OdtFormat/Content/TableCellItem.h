#pragma once
#ifndef ODT_CONTENT_TABLE_CELL_ITEM_INCLUDE_H_
#define ODT_CONTENT_TABLE_CELL_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <boost/shared_ptr.hpp>
#include "TableCellItemBase.h"


namespace Odt
{
	namespace Content
	{
		class CoveredTableCell;
		class TableCell;

		class TableCellItem : public WritingElement
		{
		public:
			TableCellItem();
			virtual ~TableCellItem();
			TableCellItem(const CoveredTableCell& coveredTableCell);
			TableCellItem(const TableCell& tableCell);
			explicit TableCellItem(const XML::XNode& node);
			const TableCellItem& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool hasNote() const;
			const bool hasList() const;

		public:
			const int getNumCol() const;
			void setNumCol(const int num);

		public:
			const int getCountHMerge() const;

		public:
			template<class T>	const bool is() const {	return typeid(*m_item) == typeid(T);}
			template<class T> T& as() {return static_cast<T&>(*m_item);}
			template<class T> const T& as() const {return static_cast<const T&>(*m_item);}

		private:
			boost::shared_ptr<TableCellItemBase>		m_item;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_CELL_ITEM_INCLUDE_H_