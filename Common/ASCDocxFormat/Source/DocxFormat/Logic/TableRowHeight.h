#pragma once
#ifndef OOX_LOGIC_TABLE_ROW_HEIGHT_INCLUDE_H_
#define OOX_LOGIC_TABLE_ROW_HEIGHT_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "./../Unit.h"
#include "./../Limit/TableRowHeightRule.h"


namespace OOX
{
	namespace Logic
	{
		class TableRowHeight : public WritingElement
		{
		public:
			TableRowHeight();
			virtual ~TableRowHeight();
			explicit TableRowHeight(const XML::XNode& node);
			const TableRowHeight& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UnitDx>										    Height;
			nullable_property<std::string, Limit::TableRowHeightRule>	Type;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_ROW_HEIGHT_INCLUDE_H_