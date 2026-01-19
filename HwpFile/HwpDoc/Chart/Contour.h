#ifndef CONTOUR_H
#define CONTOUR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CContour : public IChartObject
{
	CHART_INTEGER m_nDisplayType;
public:
	CContour();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // CONTOUR_H
