#pragma once
#ifndef OOX_LOGIC_TABLE_ROW_PROPERTIES_INCLUDE_H_
#define OOX_LOGIC_TABLE_ROW_PROPERTIES_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "TableRowHeight.h"


namespace OOX
{
	namespace Logic
	{
		class TableRowProperties : public WritingElement
		{
		public:
			TableRowProperties();
			virtual ~TableRowProperties();
			explicit TableRowProperties(const XML::XNode& node);
			const TableRowProperties& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<TableRowHeight>			Height;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_ROW_PROPERTIES_INCLUDE_H_