#include "TLChartSubElement.h"

namespace PPTX
{
	namespace Limit
	{
		TLChartSubElement::TLChartSubElement()
		{
			add("category");
			add("gridLegend");
			add("ptInCategory");
			add("ptInSeries");
			add("series");
		}

		const std::string TLChartSubElement::no_find() const
		{
			return "series";
		}
	} // namespace Limit
} // namespace PPTX