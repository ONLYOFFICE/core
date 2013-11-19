
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Cap.h"


namespace OOX
{
	namespace Limit
	{
		Cap::Cap()
		{
			add("rnd");
			add("sq");
			add("flat");
		}

		const std::string Cap::no_find() const
		{
			return "rnd";
		}
	} // namespace Limit
} // namespace OOX