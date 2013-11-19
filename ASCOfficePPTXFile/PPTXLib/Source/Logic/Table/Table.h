#pragma once
#ifndef PPTX_LOGIC_TABLE_INCLUDE_H_
#define PPTX_LOGIC_TABLE_INCLUDE_H_

#include "./../../WrapperWritingElement.h"
#include <vector>
#include "property.h"
#include "nullable_property.h"
#include "TableCol.h"
#include "TableRow.h"
#include "TableProperties.h"

namespace PPTX
{
	namespace Logic
	{
		class Table : public WrapperWritingElement
		{
		public:
			Table();
			virtual ~Table();			
			explicit Table(const XML::XNode& node);
			const Table& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;
			
		public:
			property<std::vector<TableCol> > TableCols;
			property<std::vector<TableRow> > TableRows;
			nullable_property<TableProperties> TableProperties;
		protected:
			virtual void FillParentPointersForChilds();
		};
	} // namespace Logic
} // namespace PPTX

#endif // PPTX_LOGIC_TABLE_INCLUDE_H_