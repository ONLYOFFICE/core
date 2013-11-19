
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FontVariant.h"


namespace Odt
{
	namespace Limit
	{
		FontVariant::FontVariant()
		{
			add("normal");
			add("small-caps");
		}

		const std::string FontVariant::no_find() const
		{
			return "normal";
		}
	} // namespace Limit
} // namespace Odt