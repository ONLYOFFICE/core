#include "AlbumLayout.h"

namespace PPTX
{
	namespace Limit
	{
		AlbumLayout::AlbumLayout()
		{
			add("1pic");
			add("1picTitle");
			add("2pic");
			add("2picTitle");
			add("4pic");
			add("4picTitle");
			add("fitToSlide");
		}

		const std::string AlbumLayout::no_find() const
		{
			return "fitToSlide";
		}
	} // namespace Limit
} // namespace PPTX