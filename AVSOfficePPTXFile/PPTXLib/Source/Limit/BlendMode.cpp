#include "BlendMode.h"

namespace PPTX
{
	namespace Limit
	{
		BlendMode::BlendMode()
		{
			add("darken");
			add("lighten");
			add("mult");
			add("over");
			add("screen");
		}

		const std::string BlendMode::no_find() const
		{
			return "screen";
		}
	} // namespace Limit
} // namespace PPTX