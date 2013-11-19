#include "ParaBuildType.h"

namespace PPTX
{
	namespace Limit
	{
		ParaBuildType::ParaBuildType()
		{
			add("allAtOnce");
			add("cust");
			add("p");
			add("whole");
		}

		const std::string ParaBuildType::no_find() const
		{
			return "whole";
		}
	} // namespace Limit
} // namespace PPTX