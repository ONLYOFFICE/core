#include "PlaceholderType.h"

namespace PPTX
{
	namespace Limit
	{
		PlaceholderType::PlaceholderType()
		{
			add("body");
			add("chart");
			add("clipArt");
			add("ctrTitle");
			add("dgm");
			add("dt");
			add("ftr");
			add("hdr");
			add("media");
			add("obj");
			add("pic");
			add("sldImg");
			add("sldNum");
			add("subTitle");
			add("tbl");
			add("title");
 		}

		const std::string PlaceholderType::no_find() const
		{
			return "obj";
		}
	} // namespace Limit
} // namespace PPTX