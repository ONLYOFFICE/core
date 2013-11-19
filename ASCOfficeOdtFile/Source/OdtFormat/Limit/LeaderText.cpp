
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "LeaderText.h"


namespace Odt
{
	namespace Limit
	{
		LeaderText::LeaderText()
		{
			add(".");
		}

		const std::string LeaderText::no_find() const
		{
			return ".";
		}
	} // namespace Limit
} // namespace Odt