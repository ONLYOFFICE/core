#include "Conformance.h"

namespace PPTX
{
	namespace Limit
	{
		Conformance::Conformance()
		{
			add("strict");
			add("transitional");
		}

		const std::string Conformance::no_find() const
		{
			return "transitional";
		}
	} // namespace Limit
} // namespace PPTX