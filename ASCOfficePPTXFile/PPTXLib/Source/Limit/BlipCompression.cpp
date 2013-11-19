#include "BlipCompression.h"

namespace PPTX
{
	namespace Limit
	{
		BlipCompression::BlipCompression()
		{
			add("none");
			add("email");
			add("hqprint");
			add("print");
			add("screen");
		}

		const std::string BlipCompression::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX