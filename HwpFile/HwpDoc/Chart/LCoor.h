#ifndef LCOOR_H
#define LCOOR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CLCoor : public IChartObject
{
	long m_lX;
	long m_lY;
public:
	CLCoor();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // LCOOR_H
