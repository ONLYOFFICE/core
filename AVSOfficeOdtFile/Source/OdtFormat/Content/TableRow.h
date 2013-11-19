#pragma once
#ifndef ODT_CONTENT_TABLE_ROW_INCLUDE_H_
#define ODT_CONTENT_TABLE_ROW_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "property.h"
#include "TableCellItem.h"


namespace Odt
{
	namespace Content
	{
		class TableRow : public WritingElement
		{
		public:
			TableRow();
			virtual ~TableRow();
			explicit TableRow(const XML::XNode& node);
			const TableRow& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			const bool hasNote() const;
			const bool hasList() const;

		public:
			const TableCellItem getCellByNumCol(const int num) const;

		public:
			property<std::vector<TableCellItem> >		Cells;
			nullable_property<std::string> StyleName;
			nullable_property<std::string> DefaultCellStyleName;

		private:
			bool m_hasNote;
			bool m_hasList;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_ROW_INCLUDE_H_