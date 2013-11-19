#pragma once
#ifndef ODT_LOGIC_LIST_LEVEL_STYLE_ITEM_INCLUDE_H_
#define ODT_LOGIC_LIST_LEVEL_STYLE_ITEM_INCLUDE_H_

#include "./../WritingElement.h"
#include <string>
#include "property.h"
#include "nullable_property.h"
#include "Properties.h"


namespace Odt
{
	namespace Logic
	{
		class ListLevelStyleItem : public WritingElement
		{
		public:
			property<int, setter::only_positive<int> >	Level;
			nullable_property<std::string>				Style;
			property<Properties>						Properties;			
		};
	} // namespace Logic
} // namespace Odt

#endif // ODT_LOGIC_LIST_LEVEL_STYLE_ITEM_INCLUDE_H_