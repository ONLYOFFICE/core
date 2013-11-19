
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "FontFamilyGeneric.h"


namespace Odt
{
	namespace Limit
	{
		FontFamilyGeneric::FontFamilyGeneric()
		{
			add("roman");
			add("script");
			add("swiss");
			add("system");
		}

		const std::string FontFamilyGeneric::no_find() const
		{
			return "roman";
		}
	} // namespace Limit
} // namespace Odt