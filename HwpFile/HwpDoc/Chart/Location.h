#ifndef LOCATION_H
#define LOCATION_H

#include "Rect.h"

namespace HWP { namespace CHART
{
class CLocation
{
	int m_nLocationType;
	CRect m_oRect;
	bool m_bVisible;
public:
	CLocation();
};
}}

#endif // LOCATION_H
