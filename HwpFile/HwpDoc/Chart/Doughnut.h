#ifndef DOUGHNUT_H
#define DOUGHNUT_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CDoughnut : public IChartObject
{
	int m_nSides;
	SINGLE m_snInteriorRatio;
public:
	CDoughnut();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // DOUGHNUT_H
