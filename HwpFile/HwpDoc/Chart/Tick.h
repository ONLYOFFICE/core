#ifndef TICK_H
#define TICK_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CTick : public IChartObject
{
	CHART_SINGLE m_snLength;
	CHART_INTEGER m_nStyle;
public:
	CTick();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // TICK_H
