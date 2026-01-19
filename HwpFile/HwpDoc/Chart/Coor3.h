#ifndef COOR3_H
#define COOR3_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CCoor3 : public IChartObject
{
	CHART_SINGLE m_snX;
	CHART_SINGLE m_snY;
	CHART_SINGLE m_snZ;
public:
	CCoor3();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // COOR3_H
