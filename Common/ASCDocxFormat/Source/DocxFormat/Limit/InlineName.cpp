
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "InlineName.h"


namespace OOX
{
	namespace Limit
	{

		InlineName::InlineName()
		{
			add("inline");
			add("anchor");
		}


		const std::string InlineName::no_find() const
		{
			return "inline";
		}

	} // namespace Limit
} // namespace OOX