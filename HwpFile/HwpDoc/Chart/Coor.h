#ifndef COOR_H
#define COOR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CCoor : public IChartObject
{
	CHART_SINGLE m_snX;
	CHART_SINGLE m_snY;
public:
	CCoor();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // COOR_H
