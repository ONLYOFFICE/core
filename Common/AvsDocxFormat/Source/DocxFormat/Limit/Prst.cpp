
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Prst.h"


namespace OOX
{
	namespace Limit
	{
		Prst::Prst()
		{
			add("rect");
		}

		const std::string Prst::no_find() const
		{
			return "rect";
		}
	} // namespace Limit
} // namespace OOX