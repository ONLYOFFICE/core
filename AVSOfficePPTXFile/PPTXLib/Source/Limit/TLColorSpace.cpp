#include "TLColorSpace.h"

namespace PPTX
{
	namespace Limit
	{
		TLColorSpace::TLColorSpace()
		{
			add("rgb");
			add("hsl");
		}

		const std::string TLColorSpace::no_find() const
		{
			return "rgb";
		}
	} // namespace Limit
} // namespace PPTX