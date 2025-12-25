#ifndef PIE_H
#define PIE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CPie : public IChartObject
{
	SINGLE m_snThicknessRatio;
	SINGLE m_snTopRadiusRatio;
public:
	CPie();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // PIE_H
