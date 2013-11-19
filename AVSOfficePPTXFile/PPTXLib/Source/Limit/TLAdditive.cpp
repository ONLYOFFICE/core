#include "TLAdditive.h"

namespace PPTX
{
	namespace Limit
	{
		TLAdditive::TLAdditive()
		{
			add("base");
			add("mult");
			add("none");
			add("repl");
			add("sum");
		}

		const std::string TLAdditive::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX