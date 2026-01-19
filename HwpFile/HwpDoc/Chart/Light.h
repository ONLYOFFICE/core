#ifndef LIGHT_H
#define LIGHT_H

#include "Collection.h"

namespace HWP { namespace CHART
{
class CLightSource : public IChartObject
{
	CHART_SINGLE m_snX;
	CHART_SINGLE m_snY;
	CHART_SINGLE m_snZ;
	CHART_SINGLE m_snIntensity;
public:
	CLightSource();

	bool Read(CChartStream& oStream) override;
};

using CLightSources = CCollection<CLightSource>;

class CLight : public IChartObject
{
	CHART_SINGLE m_snAmbientIntensity;
	CHART_SINGLE m_snEdgeIntensity;
	CHART_BOOLEAN m_bEdgeVisible;
	CLightSources m_oLightSources;
public:
	CLight();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // LIGHT_H
