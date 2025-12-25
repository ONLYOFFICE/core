#ifndef LOCATION_H
#define LOCATION_H

#include "Rect.h"

namespace HWP { namespace CHART
{
class CLocation : public IChartObject
{
	int m_nLocationType;
	CRect m_oRect;
	bool m_bVisible;
public:
	CLocation();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // LOCATION_H
