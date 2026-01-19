#ifndef LCOOR_H
#define LCOOR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CLCoor : public IChartObject
{
	CHART_LONG m_lX;
	CHART_LONG m_lY;
public:
	CLCoor();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // LCOOR_H
