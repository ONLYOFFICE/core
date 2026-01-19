#ifndef XYZ_H
#define XYZ_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CXYZ : public IChartObject
{
	CHART_BOOLEAN m_bAutomatic;
	CHART_DOUBLE m_dxIntersection;
	CHART_DOUBLE m_dyIntersection;
	CHART_DOUBLE m_dzIntersection;
public:
	CXYZ();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // XYZ_H
