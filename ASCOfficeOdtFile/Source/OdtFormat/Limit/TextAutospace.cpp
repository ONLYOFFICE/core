
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "TextAutospace.h"


namespace Odt
{
	namespace Limit
	{
		TextAutospace::TextAutospace()
		{
			add("ideograph-alpha");
		}

		const std::string TextAutospace::no_find() const
		{
			return "ideograph-alpha";
		}
	} // namespace Limit
} // namespace Odt