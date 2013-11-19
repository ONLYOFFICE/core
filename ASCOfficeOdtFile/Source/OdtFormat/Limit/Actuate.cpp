
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "Actuate.h"


namespace Odt
{
	namespace Limit
	{
		Actuate::Actuate()
		{
			add("onLoad");
		}

		const std::string Actuate::no_find() const
		{
			return "onLoad";
		}
	} // namespace Limit
} // namespace Odt