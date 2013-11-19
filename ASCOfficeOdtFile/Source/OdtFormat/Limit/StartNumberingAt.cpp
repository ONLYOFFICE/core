
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "StartNumberingAt.h"


namespace Odt
{
	namespace Limit
	{
		StartNumberingAt::StartNumberingAt()
		{
			add("document");
			add("page");
		}

		const std::string StartNumberingAt::no_find() const
		{
			return "document";
		}
	} // namespace Limit
} // namespace Odt