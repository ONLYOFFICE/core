#pragma once
#ifndef OOX_STYLES_TC_MAR_INCLUDE_H_
#define OOX_STYLES_TC_MAR_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "nullable_property.h"
#include "TableCellMargin.h"

namespace OOX
{
	namespace Logic
	{
		class TCMar : public WritingElement
		{
		public:
			TCMar();
			virtual ~TCMar();
			explicit TCMar(const XML::XNode& node);
			const TCMar& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			nullable_property<TableCellMargin>		Top;
			nullable_property<TableCellMargin>		Left;
			nullable_property<TableCellMargin>		Bottom;
			nullable_property<TableCellMargin>		Right;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_STYLES_TC_MAR_INCLUDE_H_