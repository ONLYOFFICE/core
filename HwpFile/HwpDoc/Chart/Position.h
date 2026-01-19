#ifndef POSITION_H
#define POSITION_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CPosition : public IChartObject
{
	CHART_BOOLEAN m_bExcluded;
	CHART_BOOLEAN m_bHidden;
	CHART_INTEGER m_nOrder;
	CHART_INTEGER m_nStackOrder;
public:
	CPosition();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // POSITION_H
