#ifndef FRAME_H
#define FRAME_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CFrame
{
	int m_nStyle;
	SINGLE m_snWidth;
	CVtColor m_oFrameColor;
	CVtColor m_oSpaceColor;
public:
	CFrame();
};
}}

#endif // FRAME_H
