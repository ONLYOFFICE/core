
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Position.h"


namespace OOX
{
	namespace Limit
	{
		Position::Position()
		{
			add("absolute");
		}

		const std::string Position::no_find() const
		{
			return "absolute";
		}
	} // namespace Limit
} // namespace OOX