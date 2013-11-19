#pragma once
#ifndef OOX_LOGIC_TABLE_CELL_WIDTH_INCLUDE_H_
#define OOX_LOGIC_TABLE_CELL_WIDTH_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "./../Unit.h"
#include "./../Limit/TableCellWidthType.h"


namespace OOX
{
	namespace Logic
	{
		class TableCellWidth : public WritingElement
		{
		public:
			TableCellWidth();
			virtual ~TableCellWidth();
			explicit TableCellWidth(const XML::XNode& node);
			const TableCellWidth& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UnitDx>																	Width;
			property<std::string, Limit::TableCellWidthType>	Type;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_CELL_WIDTH_INCLUDE_H_