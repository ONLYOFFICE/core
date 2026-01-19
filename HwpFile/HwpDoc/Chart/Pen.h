#ifndef PEN_H
#define PEN_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CPen : public IChartObject
{
	CHART_INTEGER m_nCap;
	CHART_INTEGER m_nJoin;
	CHART_SINGLE m_snLimit;
	CHART_INTEGER m_nStyle;
	CHART_SINGLE m_snWidth;
	CVtColor m_oVtColor;
public:
	CPen();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // PEN_H
