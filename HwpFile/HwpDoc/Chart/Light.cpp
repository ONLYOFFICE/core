#include "Light.h"

namespace HWP { namespace CHART
{
CLightSource::CLightSource()
{

}

bool CLightSource::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snX) && oStream.ReadSingle(m_snY) &&
	       oStream.ReadSingle(m_snZ) && oStream.ReadSingle(m_snIntensity);
}

CLight::CLight()
{

}

bool CLight::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snAmbientIntensity) && oStream.ReadSingle(m_snEdgeIntensity) &&
	       oStream.ReadBoolean(m_bEdgeVisible) && m_oLightSources.Read(oStream);
}
}}
