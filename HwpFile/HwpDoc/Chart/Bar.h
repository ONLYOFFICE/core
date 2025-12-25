#ifndef BAR_H
#define BAR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CBar : public IChartObject
{
	int m_nSides;
	SINGLE m_snTopRatio;
public:
	CBar();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // BAR_H
