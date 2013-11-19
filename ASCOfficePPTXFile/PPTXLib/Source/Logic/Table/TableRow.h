#pragma once
#ifndef PPTX_LOGIC_TABLEROW_INCLUDE_H_
#define PPTX_LOGIC_TABLEROW_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include "property.h"
#include <vector>
#include "TableCell.h"

namespace PPTX
{
	namespace Logic
	{
		class TableRow : public WrapperWritingElement
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
			property<int> Height;
			property<std::vector<TableCell> > Cells;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLEROW_INCLUDE_H_