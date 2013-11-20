
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FldCharType.h"


namespace OOX
{
	namespace Limit
	{
		FldCharType::FldCharType()
		{
			add("begin");
			add("end");
			add("separate");
		}

		const std::string FldCharType::no_find() const
		{
			return "begin";
		}
	} // namespace Limit
} // namespace OOX