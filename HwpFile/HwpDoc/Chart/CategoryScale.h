#ifndef CATEGORYSCALE_H
#define CATEGORYSCALE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CCategoryScale : public IChartObject
{
	CHART_BOOLEAN m_bAuto;
	CHART_INTEGER m_nDivisionsPerLabel;
	CHART_INTEGER m_nDivisionsPerTick;
	CHART_BOOLEAN m_bLabelTick;
public:
	CCategoryScale();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // CATEGORYSCALE_H
