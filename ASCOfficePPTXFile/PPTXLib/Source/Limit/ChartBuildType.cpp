#include "ChartBuildType.h"

namespace PPTX
{
	namespace Limit
	{
		ChartBuildType::ChartBuildType()
		{
			add("allAtOnce");
			add("category");
			add("categoryEl");
			add("series");
			add("seriesEl");
		}

		const std::string ChartBuildType::no_find() const
		{
			return "allAtOnce";
		}
	} // namespace Limit
} // namespace PPTX