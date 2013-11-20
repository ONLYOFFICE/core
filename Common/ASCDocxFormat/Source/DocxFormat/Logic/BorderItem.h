#pragma once
#ifndef OOX_LOGIC_BORDER_ITEM_INCLUDE_H_
#define OOX_LOGIC_BORDER_ITEM_INCLUDE_H_

#include <string>
#include "Color.h" 
#include "./../Limit/BorderValue.h"
#include "./../Unit.h"
#include "property.h"
#include "nullable_property.h"

namespace OOX
{
	namespace Logic
	{
		class BorderItem
		{
		public:
			BorderItem();
			~BorderItem();
		
		public:			
			property<std::string, Limit::BorderValue>	Value;
			nullable_property<UnitDx>					Sz;
			nullable_property<int>						Space;
			nullable_property<Logic::Color>				Color;
			nullable_property<std::string>				ThemeColor;
		};
	} // namespace Logic
} // namespace OOX

#endif // OOX_LOGIC_BORDER_ITEM_INCLUDE_H_