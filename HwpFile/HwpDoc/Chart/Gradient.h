#ifndef GRADIENT_H
#define GRADIENT_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CGradient : public IChartObject
{
	CVtColor m_oFromColor;
	CHART_INTEGER m_nStyle;
	CVtColor m_oToColor;
public:
	CGradient();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // GRADIENT_H
