
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TextPathScale.h"


namespace Odt
{
	namespace Limit
	{
		TextPathScale::TextPathScale()
		{
			add("path");
		}

		const std::string TextPathScale::no_find() const
		{
			return "path";
		}
	} // namespace Limit
} // namespace Odt