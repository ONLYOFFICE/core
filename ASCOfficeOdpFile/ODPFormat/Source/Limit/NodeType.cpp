#include "NodeType.h"

namespace Odp
{
	namespace Limit
	{
		NodeType::NodeType()
		{
			add("timing-root");
			add("main-sequence");
			add("on-click");
			add("after-previous");
			add("default");
			add("interactive-sequence");
			add("with-previous");
		}

		const std::string NodeType::no_find() const
		{
			return "default";
		}
	} // namespace Limit
} // namespace Odp