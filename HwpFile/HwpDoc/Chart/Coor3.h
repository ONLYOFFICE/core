#ifndef COOR3_H
#define COOR3_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CCoor3 : public IChartObject
{
	SINGLE m_snX;
	SINGLE m_snY;
	SINGLE m_snZ;
public:
	CCoor3();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // COOR3_H
