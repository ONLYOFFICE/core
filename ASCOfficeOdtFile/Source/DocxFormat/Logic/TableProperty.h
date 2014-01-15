#pragma once
#ifndef OOX_LOGIC_TABLE_PROPERTY_INCLUDE_H_
#define OOX_LOGIC_TABLE_PROPERTY_INCLUDE_H_

#include "./../WritingElement.h"
#include "TableWidth.h"
#include "TableBorders.h"
#include "TableInd.h"
#include "TableCellMar.h"
#include "nullable_property.h"

namespace OOX
{
	namespace Logic
	{
		class TableProperty : public WritingElement
		{
		public:
			TableProperty();
			virtual ~TableProperty();
			explicit TableProperty(const XML::XNode& node);
			const TableProperty& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:

			nullable_property<std::string>		Style;
			nullable_property<TableWidth>		Width;
			nullable_property<std::string>		Look;
			nullable_property<TableInd>			Ind;			
			nullable_property<TableCellMar>		CellMar;
			nullable_property<std::string>		Layout;
		
			nullable_property<TableBorders>		tblBorders;

			int									tblStyleRowBandSize;
			int									tblStyleColBandSize;

			int									firstRow;
			int									lastRow; 
			int									firstColumn;
			int									lastColumn;
			int									noHBand;
			int									noVBand;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_PROPERTY_INCLUDE_H_