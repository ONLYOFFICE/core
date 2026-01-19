#ifndef BAR_H
#define BAR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CBar : public IChartObject
{
	CHART_INTEGER m_nSides;
	CHART_SINGLE m_snTopRatio;
public:
	CBar();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // BAR_H
