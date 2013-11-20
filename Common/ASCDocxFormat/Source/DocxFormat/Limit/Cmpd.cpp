
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Cmpd.h"


namespace OOX
{
	namespace Limit
	{
		Cmpd::Cmpd()
		{
			add("sng");
			add("thickThin");
			add("tri");
			add("dbl");
		}

		const std::string Cmpd::no_find() const
		{
			return "sng";
		}
	} // namespace Limit
} // namespace OOX