#pragma once
#ifndef OOX_LOGIC_CELL_MARGIN_ITEM_INCLUDE_H_
#define OOX_LOGIC_CELL_MARGIN_ITEM_INCLUDE_H_

#include <string>
#include "./../Unit.h"
#include "property.h"


namespace OOX
{
	namespace Logic
	{
		class CellMarginItem
		{
		public:
			CellMarginItem();
			CellMarginItem(int width, const std::string& typeMar);
			~CellMarginItem();
		
		public:			
			property<int>			W;
			property<std::string>	Type;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_CELL_MARGIN_ITEM_INCLUDE_H_