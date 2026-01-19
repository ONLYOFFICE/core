#ifndef STATLINE_H
#define STATLINE_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CStatLine : public IChartObject
{
	CHART_INTEGER m_nFlags;
	CHART_INTEGER m_nStyle;
	CVtColor m_oVtColor;
	CHART_SINGLE m_snWidth;
public:
	CStatLine();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // STATLINE_H
