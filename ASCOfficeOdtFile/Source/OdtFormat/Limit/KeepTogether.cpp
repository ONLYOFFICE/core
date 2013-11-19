
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "KeepTogether.h"


namespace Odt
{
	namespace Limit
	{
		KeepTogether::KeepTogether()
		{
			add("auto");
			add("true");
			add("always");
		}

		const std::string KeepTogether::no_find() const
		{
			return "auto";
		}
	} // namespace Limit
} // namespace Odt