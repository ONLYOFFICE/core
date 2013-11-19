#pragma once
#ifndef ODT_CONTENT_TABLE_CELL_INCLUDE_H_
#define ODT_CONTENT_TABLE_CELL_INCLUDE_H_

#include "TableCellItemBase.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "Paragraph.h"
#include "./../Limit/TableCellType.h"
#include <vector>
#include "Text.h"


namespace Odt
{
	namespace Content
	{
		class TableCell : public TableCellItemBase
		{
		public:
			TableCell();
			virtual ~TableCell();
			explicit TableCell(const XML::XNode& node);
			const TableCell& operator =(const XML::XNode& node);

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
			const bool isFromHMerged() const;

		public:
			nullable_property<std::string>												Style;
			nullable_property<std::string, Limit::TableCellType>	Type;
			property<std::vector<Text> >													Texts;
			nullable_property<size_t>															ColumnsSpanned;
			nullable_property<size_t>															RowsSpanned;

		private:
			bool m_hasNote;
			bool m_hasList;
		};
	} // namespace Content
} // namespace Odt

#endif // ODT_CONTENT_TABLE_CELL_INCLUDE_H_