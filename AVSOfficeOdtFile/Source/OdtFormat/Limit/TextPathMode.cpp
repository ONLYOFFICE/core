
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TextPathMode.h"


namespace Odt
{
	namespace Limit
	{
		TextPathMode::TextPathMode()
		{
			add("shape");
		}

		const std::string TextPathMode::no_find() const
		{
			return "shape";
		}
	} // namespace Limit
} // namespace Odt