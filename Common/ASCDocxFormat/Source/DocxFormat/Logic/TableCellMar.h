#pragma once
#ifndef OOX_STYLES_TABLE_CELL_MAR_INCLUDE_H_
#define OOX_STYLES_TABLE_CELL_MAR_INCLUDE_H_

#include "./../WritingElement.h"
#include "nullable_property.h"
#include "TableCellMargin.h"

namespace OOX
{
	namespace Logic
	{
		class TableCellMar : public WritingElement
		{
		public:
			TableCellMar();
			virtual ~TableCellMar();
			explicit TableCellMar(const XML::XNode& node);
			const TableCellMar& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<TableCellMargin>		Top;
			nullable_property<TableCellMargin>		Left;
			nullable_property<TableCellMargin>		Bottom;
			nullable_property<TableCellMargin>		Right;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_STYLES_TABLE_CELL_MAR_INCLUDE_H_