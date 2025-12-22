#ifndef LIGHT_H
#define LIGHT_H

#include "../Common/Common.h"

namespace HWP { namespace CHART
{
class CLightSource
{
	SINGLE m_snX;
	SINGLE m_snY;
	SINGLE m_snZ;
	SINGLE m_snIntensity;
public:
	CLightSource();
};

using CLightSources = ICollection<CLightSource>;

class CLight
{
	SINGLE m_snAmbientIntensity;
	SINGLE m_snEdgeIntensity;
	bool m_bEdgeVisible;
	CLightSources m_oLightSources;
public:
	CLight();
};
}}

#endif // LIGHT_H
