
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "WritingMode.h"


namespace Odt
{
	namespace Limit
	{
		WritingMode::WritingMode()
		{
			add("lr-tb");
			add("page");
		}

		const std::string WritingMode::no_find() const
		{
			return "lr-tb";
		}
	} // namespace Limit
} // namespace Odt