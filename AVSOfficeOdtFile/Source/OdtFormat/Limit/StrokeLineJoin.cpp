
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "StrokeLineJoin.h"


namespace Odt
{
	namespace Limit
	{
		StrokeLineJoin::StrokeLineJoin()
		{
			add("miter");
		}

		const std::string StrokeLineJoin::no_find() const
		{
			return "miter";
		}
	} // namespace Limit
} // namespace Odt