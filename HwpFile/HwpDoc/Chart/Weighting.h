#ifndef WEIGHTING_H
#define WEIGHTING_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CWeighting : public IChartObject
{
	int m_nBasis;
	int m_nStyle;
public:
	CWeighting();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // WEIGHTING_H
