#include "ChartBuild.h"

namespace PPTX
{
	namespace Limit
	{
		ChartBuild::ChartBuild()
		{
			add("allPts");
			add("category");
			add("gridLegend");
			add("ptInCategory");
			add("ptInSeries");
			add("series");
		}

		const std::string ChartBuild::no_find() const
		{
			return "allPts";
		}
	} // namespace Limit
} // namespace PPTX