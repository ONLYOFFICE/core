
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "BorderModel.h"


namespace Odt
{
	namespace Limit
	{
		BorderModel::BorderModel()
		{
			add("collapsing");
			add("separating");
		}

		const std::string BorderModel::no_find() const
		{
			return "collapsing";
		}
	} // namespace Limit
} // namespace Odt