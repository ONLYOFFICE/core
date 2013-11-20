#pragma once
#ifndef OOX_LOGIC_TABLE_CELL_PROPERTIES_INCLUDE_H_
#define OOX_LOGIC_TABLE_CELL_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include "TableCellWidth.h"
#include "TableCellBorders.h"
#include "Color.h"
#include "property.h"
#include "VMerge.h"
#include "TCMar.h"
#include "Shading.h"
#include "./../Limit/VerticalAlign.h"

namespace OOX
{
	namespace Logic
	{
		class TableCellProperties : public WritingElement
		{
		public:
			TableCellProperties();
			virtual ~TableCellProperties();
			explicit TableCellProperties(const XML::XNode& node);
			const TableCellProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<TableCellWidth>						Width;
			nullable_property<TableCellBorders>				tblBorders;
			nullable_property<Color>						Background;
			nullable_property<Shading>                      Shading; //TODO: Read only now
			nullable_property<VMerge>						VMerge;
			nullable_property<int>							GridSpan; 
			property<int>									CountVMerge;
			nullable_property<TCMar>					    CellMar;
			nullable_property<std::string, Limit::VerticalAlign> VAlign;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_CELL_PROPERTIES_INCLUDE_H_