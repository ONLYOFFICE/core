#include "Light.h"

namespace HWP { namespace CHART
{
CLightSource::CLightSource()
{

}

bool CLightSource::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snX) && oStream.ReadShort(m_snY) &&
	       oStream.ReadShort(m_snZ) && oStream.ReadShort(m_snIntensity);
}

CLight::CLight()
{

}

bool CLight::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snAmbientIntensity) && oStream.ReadShort(m_snEdgeIntensity) &&
	       oStream.ReadBool(m_bEdgeVisible) && m_oLightSources.Read(oStream);
}
}}
