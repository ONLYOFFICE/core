#ifndef LOCATION_H
#define LOCATION_H

#include "Rect.h"

namespace HWP { namespace CHART
{
class CLocation : public IChartObject
{
	CHART_INTEGER m_nLocationType;
	CRect m_oRect;
	CHART_BOOLEAN m_bVisible;
public:
	CLocation();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // LOCATION_H
