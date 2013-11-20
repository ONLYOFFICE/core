#pragma once
#ifndef OOX_LOGIC_TABLE_CELL_MARGIN_INCLUDE_H_
#define OOX_LOGIC_TABLE_CELL_MARGIN_INCLUDE_H_

#include "./../WritingElement.h"
#include "CellMarginItem.h"
#include "property.h"

namespace OOX
{
	namespace Logic
	{
		class TableCellMargin : public WritingElement
		{
		public:
			TableCellMargin();
			virtual ~TableCellMargin();
			explicit TableCellMargin(const XML::XNode& node);
			TableCellMargin(const std::string& name, const CellMarginItem& margin);
			const TableCellMargin& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string> Name;
			property<CellMarginItem>  Margin;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_CELL_MARGIN_INCLUDE_H_