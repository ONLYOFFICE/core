
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "VMergeVal.h"


namespace OOX
{
	namespace Limit
	{
		VMergeVal::VMergeVal()
		{
			add("restart");
			add("continue");
		}

		const std::string VMergeVal::no_find() const
		{
			return "restart";
		}
	} // namespace Limit
} // namespace OOX