#pragma once
#ifndef OOX_LOGIC_GRID_COLUMN_INCLUDE_H_
#define OOX_LOGIC_GRID_COLUMN_INCLUDE_H_

#include "./../WritingElement.h"
#include "./../Unit.h"

namespace OOX
{
	namespace Logic
	{
		class GridColumn : public WritingElement
		{
		public:
			GridColumn();
			virtual ~GridColumn();
			explicit GridColumn(const XML::XNode& node);
			const GridColumn& operator =(const XML::XNode& node);

		public:
			virtual void fromXML(const XML::XNode& node);
			virtual const XML::XNode toXML() const;

		public:
			property<UnitDx>	Width;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_GRID_COLUMN_INCLUDE_H_