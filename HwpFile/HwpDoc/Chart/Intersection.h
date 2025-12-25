#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CIntersection : public IChartObject
{
	bool m_bAuto;
	int m_nAxisId;
	int m_nIndex;
	bool m_bLabelsInsidePlot;
	double m_dPoint;
public:
	CIntersection();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // INTERSECTION_H
