#ifndef VALUESCALE_H
#define VALUESCALE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CValueScale : public IChartObject
{
	CHART_BOOLEAN m_bAuto;
	CHART_INTEGER m_nMajorDivision;
	CHART_DOUBLE m_dMaximum;
	CHART_DOUBLE m_dMinimum;
	CHART_INTEGER m_nMinorDivision;
public:
	CValueScale();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // VALUESCALE_H
