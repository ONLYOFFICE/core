
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "SectPrType.h"


namespace OOX
{
	namespace Limit
	{
		SectPrType::SectPrType()
		{
			add("continuous");
			add("nextColumn");
			add("nextPage");
			add("evenPage");
			add("oddPage");
		}

		const std::string SectPrType::no_find() const
		{
			return "nextPage";
		}
	} // namespace Limit
} // namespace OOX