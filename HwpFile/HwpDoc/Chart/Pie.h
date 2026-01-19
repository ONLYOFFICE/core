#ifndef PIE_H
#define PIE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CPie : public IChartObject
{
	CHART_SINGLE m_snThicknessRatio;
	CHART_SINGLE m_snTopRadiusRatio;
public:
	CPie();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // PIE_H
