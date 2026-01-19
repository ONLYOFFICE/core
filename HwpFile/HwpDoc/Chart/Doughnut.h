#ifndef DOUGHNUT_H
#define DOUGHNUT_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CDoughnut : public IChartObject
{
	CHART_INTEGER m_nSides;
	CHART_SINGLE m_snInteriorRatio;
public:
	CDoughnut();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // DOUGHNUT_H
