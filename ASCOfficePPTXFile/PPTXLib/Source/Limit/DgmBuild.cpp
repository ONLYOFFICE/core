#include "DgmBuild.h"

namespace PPTX
{
	namespace Limit
	{
		DgmBuild::DgmBuild()
		{
			add("bg");
			add("sp");
		}

		const std::string DgmBuild::no_find() const
		{
			return "sp";
		}
	} // namespace Limit
} // namespace PPTX