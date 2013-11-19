
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BorderType.h"


namespace Odt
{
	namespace Limit
	{
		BorderType::BorderType()
		{
			add("solid");
			add("double");
		}

		const std::string BorderType::no_find() const
		{
			return "solid";
		}
	} // namespace Limit
} // namespace Odt