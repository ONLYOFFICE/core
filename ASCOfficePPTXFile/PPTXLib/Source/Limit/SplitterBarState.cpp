#include "SplitterBarState.h"

namespace PPTX
{
	namespace Limit
	{
		SplitterBarState::SplitterBarState()
		{
			add("maximized");
			add("minimized");
			add("restored");
		}

		const std::string SplitterBarState::no_find() const
		{
			return "restored";
		}
	} // namespace Limit
} // namespace PPTX