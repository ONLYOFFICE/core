#ifndef FRAME_H
#define FRAME_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CFrame : public IChartObject
{
	CHART_INTEGER m_nStyle;
	CHART_SINGLE m_snWidth;
	CVtColor m_oFrameColor;
	CVtColor m_oSpaceColor;
public:
	CFrame();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // FRAME_H
