
// auto inserted precompiled begin
#include "precompiled_docxformat.h"
// auto inserted precompiled end

#include "Dash.h"


namespace OOX
{
	namespace Limit
	{
		Dash::Dash()
		{
			add("solid");
			add("sysDash");
			add("sysDot");
			add("dash");
			add("lgDash");
			add("lgDashDot");
		}

		const std::string Dash::no_find() const
		{
			return "solid";
		}
	} // namespace Limit
} // namespace OOX