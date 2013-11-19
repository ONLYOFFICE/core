
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Background.h"


namespace Odt
{
	namespace Limit
	{
		Background::Background()
		{
			add("border");
		}

		const std::string Background::no_find() const
		{
			return "border";
		}
	} // namespace Limit
} // namespace Odt