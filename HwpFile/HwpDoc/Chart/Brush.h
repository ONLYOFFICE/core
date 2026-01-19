#ifndef BRUSH_H
#define BRUSH_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CBrush : public IChartObject
{
	CVtColor m_oFillColor;
	CHART_INTEGER m_nIndex;
	CVtColor m_oPatternColor;
	CHART_INTEGER m_nStyle;
public:
	CBrush();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // BRUSH_H
