#ifndef DATESCALE_H
#define DATESCALE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CDateScale : public IChartObject
{
	CHART_BOOLEAN m_bAuto;
	CHART_INTEGER m_nMajFreq;
	CHART_INTEGER m_nMajInt;
	CHART_DOUBLE m_dMaximum;
	CHART_DOUBLE m_dMinimum;
	CHART_INTEGER m_nMinFreq;
	CHART_INTEGER m_nMinInt;
	CHART_BOOLEAN m_bSkipWeekend;
public:
	CDateScale();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // DATESCALE_H
