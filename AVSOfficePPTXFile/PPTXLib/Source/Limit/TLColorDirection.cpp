#include "TLColorDirection.h"

namespace PPTX
{
	namespace Limit
	{
		TLColorDirection::TLColorDirection()
		{
			add("ccw");
			add("cw");
		}

		const std::string TLColorDirection::no_find() const
		{
			return "cw";
		}
	} // namespace Limit
} // namespace PPTX