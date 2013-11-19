#include "PathFillMode.h"

namespace PPTX
{
	namespace Limit
	{
		PathFillMode::PathFillMode()
		{
			add("darken");
			add("darkenLess");
			add("lighten");
			add("lightenLess");
			add("none");
			add("norm");
		}

		const std::string PathFillMode::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX