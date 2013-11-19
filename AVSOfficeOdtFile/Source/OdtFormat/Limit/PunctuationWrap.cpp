
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "PunctuationWrap.h"


namespace Odt
{
	namespace Limit
	{
		PunctuationWrap::PunctuationWrap()
		{
			add("hanging");
			add("simple");
		}

		const std::string PunctuationWrap::no_find() const
		{
			return "hanging";
		}
	} // namespace Limit
} // namespace Odt