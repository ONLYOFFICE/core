#ifndef COOR_H
#define COOR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CCoor : public IChartObject
{
	SINGLE m_snX;
	SINGLE m_snY;
public:
	CCoor();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // COOR_H
