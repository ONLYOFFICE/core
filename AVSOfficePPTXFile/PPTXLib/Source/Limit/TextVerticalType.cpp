#include "TextVerticalType.h"

namespace PPTX
{
	namespace Limit
	{
		TextVerticalType::TextVerticalType()
		{
			add("eaVert");
			add("horz");
			add("mongolianVert");
			add("vert");
			add("vert270");
			add("wordArtVert");
			add("wordArtVertRtl");
		}

		const std::string TextVerticalType::no_find() const
		{
			return "horz";
		}
	} // namespace Limit
} // namespace PPTX