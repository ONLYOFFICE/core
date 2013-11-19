
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "HrefType.h"


namespace Odt
{
	namespace Limit
	{
		HrefType::HrefType()
		{
			add("simple");
		}

		const std::string HrefType::no_find() const
		{
			return "simple";
		}
	} // namespace Limit
} // namespace Odt