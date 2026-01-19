#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CIntersection : public IChartObject
{
	CHART_BOOLEAN m_bAuto;
	CHART_INTEGER m_nAxisId;
	CHART_INTEGER m_nIndex;
	CHART_BOOLEAN m_bLabelsInsidePlot;
	CHART_DOUBLE m_dPoint;
public:
	CIntersection();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // INTERSECTION_H
