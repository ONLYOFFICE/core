#pragma once
#ifndef PPTX_LOGIC_TABLE_CELL_INCLUDE_H_
#define PPTX_LOGIC_TABLE_CELL_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "nullable_property.h"
#include "./../TxBody.h"
#include "TableCellProperties.h"
#include "../ShapeProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class TableCell : public WrapperWritingElement
		{
		public:
			TableCell();
			virtual ~TableCell();			
			explicit TableCell(const XML::XNode& node);
			const TableCell& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

			virtual void GetShapeProperties(ShapeProperties& props)const;
		public:
			nullable_property<TxBody> TxBody;
			nullable_property<TableCellProperties> CellProperties;
			nullable_property<int> RowSpan;
			nullable_property<int> GridSpan;
			nullable_property<bool> HMerge;
			nullable_property<bool> VMerge;
			nullable_property<std::string> Id;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_CELL_INCLUDE_H_