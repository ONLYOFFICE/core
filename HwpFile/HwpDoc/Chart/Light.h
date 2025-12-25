#ifndef LIGHT_H
#define LIGHT_H

#include "Collection.h"

namespace HWP { namespace CHART
{
class CLightSource : public IChartObject
{
	SINGLE m_snX;
	SINGLE m_snY;
	SINGLE m_snZ;
	SINGLE m_snIntensity;
public:
	CLightSource();

	bool Read(CHWPStream& oStream) override;
};

using CLightSources = CCollection<CLightSource>;

class CLight : public IChartObject
{
	SINGLE m_snAmbientIntensity;
	SINGLE m_snEdgeIntensity;
	bool m_bEdgeVisible;
	CLightSources m_oLightSources;
public:
	CLight();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // LIGHT_H
