
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "NumberWrappedParagraphs.h"


namespace Odt
{
	namespace Limit
	{
		NumberWrappedParagraphs::NumberWrappedParagraphs()
		{
			add("no limit");
		}

		const std::string NumberWrappedParagraphs::no_find() const
		{
			return "no limit";
		}
	} // namespace Limit
} // namespace Odt