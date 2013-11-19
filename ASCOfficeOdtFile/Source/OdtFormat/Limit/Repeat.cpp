
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Repeat.h"


namespace Odt
{
	namespace Limit
	{
		Repeat::Repeat()
		{
			add("repeat");
			add("stretch");
			add("no-repeat");
		}

		const std::string Repeat::no_find() const
		{
			return "repeat";
		}
	} // namespace Limit
} // namespace Odt