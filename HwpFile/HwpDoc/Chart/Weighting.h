#ifndef WEIGHTING_H
#define WEIGHTING_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CWeighting : public IChartObject
{
	CHART_INTEGER m_nBasis;
	CHART_INTEGER m_nStyle;
public:
	CWeighting();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // WEIGHTING_H
