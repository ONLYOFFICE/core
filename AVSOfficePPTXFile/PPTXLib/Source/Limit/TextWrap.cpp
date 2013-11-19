#include "TextWrap.h"

namespace PPTX
{
	namespace Limit
	{
		TextWrap::TextWrap()
		{
			add("none");
			add("square");
		}

		const std::string TextWrap::no_find() const
		{
			return "square";
		}
	} // namespace Limit
} // namespace PPTX