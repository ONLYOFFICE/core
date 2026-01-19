#ifndef VTCOLOR_H
#define VTCOLOR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CVtColor : public IChartObject
{
	CHART_BOOLEAN m_bAutomatic;
	CHART_INTEGER m_nBlue;
	CHART_INTEGER m_nGreen;
	CHART_INTEGER m_nRed;
	CHART_INTEGER m_nValue;
public:
	CVtColor();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // VTCOLOR_H
