#include "Flip.h"

namespace PPTX
{
	namespace Limit
	{
		Flip::Flip()
		{
			add("none");
			add("x");
			add("xy");
			add("y");			
		}

		const std::string Flip::no_find() const
		{
			return "none";
		}
	} // namespace Limit
} // namespace PPTX