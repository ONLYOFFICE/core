#include "AnimSubItem.h"

namespace Odp
{
	namespace Limit
	{
		AnimSubItem::AnimSubItem()
		{
			add("background");
			add("text");
			add("whole");
		}

		const std::string AnimSubItem::no_find() const
		{
			return "whole";
		}
	} // namespace Limit
} // namespace Odp