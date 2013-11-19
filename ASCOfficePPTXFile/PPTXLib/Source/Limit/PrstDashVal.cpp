#include "PrstDashVal.h"

namespace PPTX
{
	namespace Limit
	{
		PrstDashVal::PrstDashVal()
		{
			add("dash");
			add("dashDot");
			add("dot");
			add("lgDash");
			add("lgDashDot");
			add("lgDashDotDot");
			add("solid");
			add("sysDash");
			add("sysDashDot");
			add("sysDashDotDot");
			add("sysDot");
		}

		const std::string PrstDashVal::no_find() const
		{
			return "solid";
		}
	} // namespace Limit
} // namespace PPTX