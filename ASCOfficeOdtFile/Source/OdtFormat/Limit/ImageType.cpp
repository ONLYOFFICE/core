
// auto inserted precompiled begin
#include "precompiled_odtformat.h"
// auto inserted precompiled end

#include "ImageType.h"


namespace Odt
{
	namespace Limit
	{
		ImageType::ImageType()
		{
			add("simple");
		}

		const std::string ImageType::no_find() const
		{
			return "simple";
		}
	} // namespace Limit
} // namespace Odt