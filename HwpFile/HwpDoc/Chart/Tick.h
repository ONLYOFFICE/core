#ifndef TICK_H
#define TICK_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CTick : public IChartObject
{
	SINGLE m_snLength;
	int m_nStyle;
public:
	CTick();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // TICK_H
