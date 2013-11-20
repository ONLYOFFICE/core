
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "FillType.h"


namespace OOX
{
	namespace Limit
	{
		FillType::FillType()
		{
			add("gradient");
		}

		const std::string FillType::no_find() const
		{
			return "gradient";
		}
	} // namespace Limit
} // namespace OOX