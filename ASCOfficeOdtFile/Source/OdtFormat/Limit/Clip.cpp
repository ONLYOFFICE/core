
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Clip.h"


namespace Odt
{
	namespace Limit
	{
		Clip::Clip()
		{
			add("rect");
		}

		const std::string Clip::no_find() const
		{
			return "rect";
		}
	} // namespace Limit
} // namespace Odt