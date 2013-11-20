#pragma once
#ifndef OOX_LOGIC_TABLE_WIDTH_INCLUDE_H_
#define OOX_LOGIC_TABLE_WIDTH_INCLUDE_H_

#include "./../WritingElement.h"
#include "property.h"
#include "./../Limit/TableWidthType.h"
#include "./../Unit.h"

namespace OOX
{
	namespace Logic
	{
		class TableWidth : public WritingElement
		{
		public:
			TableWidth();
			virtual ~TableWidth();
			explicit TableWidth(const XML::XNode& node);
			const TableWidth& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UnitDx>								Width;
			property<std::string, Limit::TableWidthType>	Type;	
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_TABLE_WIDTH_INCLUDE_H_