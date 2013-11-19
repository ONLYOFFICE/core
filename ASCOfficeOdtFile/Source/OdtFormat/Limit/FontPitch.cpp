
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FontPitch.h"


namespace Odt
{
	namespace Limit
	{
		FontPitch::FontPitch()
		{
			add("variable");
			add("fixed");
		}

		const std::string FontPitch::no_find() const
		{
			return "variable";
		}
	} // namespace Limit
} // namespace Odt