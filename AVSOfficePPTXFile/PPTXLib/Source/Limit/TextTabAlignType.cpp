#include "TextTabAlignType.h"

namespace PPTX
{
	namespace Limit
	{
		TextTabAlignType::TextTabAlignType()
		{
			add("ctr");
			add("dec");
			add("l");
			add("r");
		}

		const std::string TextTabAlignType::no_find() const
		{
			return "r";
		}
	} // namespace Limit
} // namespace PPTX