
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "TrueFalse.h"


namespace OOX
{
	namespace Limit
	{
		TrueFalse::TrueFalse()
		{
			add("t");
			add("f");
		}

		const std::string TrueFalse::no_find() const
		{
			return "f";
		}
	} // namespace Limit
} // namespace OOX