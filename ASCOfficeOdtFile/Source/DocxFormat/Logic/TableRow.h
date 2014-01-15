#pragma once
#ifndef OOX_LOGIC_TABLE_ROW_INCLUDE_H_
#define OOX_LOGIC_TABLE_ROW_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include <vector>
#include "property.h"
#include "TableCell.h"
#include "TableRowProperties.h"


namespace OOX
{
	namespace Logic
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
			const TableCell getCellByNumCol(const int num) const;
			const bool isCellByNumCol(const int num) const;

		public:
			property<std::vector<TableCell> >		Cells;
			nullable_property<TableRowProperties>     Properties;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_ROW_INCLUDE_H_