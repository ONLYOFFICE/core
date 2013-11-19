#pragma once
#ifndef OOX_LOGIC_TABLE_GRID_INCLUDE_H_
#define OOX_LOGIC_TABLE_GRID_INCLUDE_H_

#include "./../WritingElement.h"
#include <vector>
#include "property.h"
#include "GridColumn.h"

namespace OOX
{
	namespace Logic
	{
		class TableGrid : public WritingElement
		{
		public:
			TableGrid();
			virtual ~TableGrid();
			explicit TableGrid(const XML::XNode& node);
			const TableGrid& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::vector<GridColumn> >	Columns;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_GRID_INCLUDE_H_