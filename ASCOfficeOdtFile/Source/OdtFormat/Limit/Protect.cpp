
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Protect.h"


namespace Odt
{
	namespace Limit
	{
		Protect::Protect()
		{
			add("size");
		}

		const std::string Protect::no_find() const
		{
			return "size";
		}
	} // namespace Limit
} // namespace Odt