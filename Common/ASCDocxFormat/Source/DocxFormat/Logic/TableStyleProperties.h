#pragma once
#ifndef OOX_LOGIC_TABLE_STYLE_PROPERTIES_INCLUDE_H_
#define OOX_LOGIC_TABLE_STYLE_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include "nullable_property.h"
#include "ParagraphProperty.h"
#include "RunProperty.h"
#include "TableProperty.h"
#include "TableRowProperties.h"
#include "TableCellProperties.h"
#include "../Limit/TableStyleType.h"

namespace OOX
{
	namespace Logic
	{
		class TableStyleProperties : public WritingElement
		{
		public:
			TableStyleProperties();
			virtual ~TableStyleProperties();
			explicit TableStyleProperties(const XML::XNode& node);
			const TableStyleProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<std::string, Limit::TableStyleType>	Type;
			nullable_property<ParagraphProperty>			ParagraphProperty;
			nullable_property<RunProperty>					RunProperty;
			nullable_property<TableProperty>				TableProperty;
			nullable_property<TableRowProperties>			TableRowProperties;
			nullable_property<TableCellProperties>			tcPr;			//	cell properties
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_STYLE_PROPERTIES_INCLUDE_H_