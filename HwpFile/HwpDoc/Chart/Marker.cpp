#include "Marker.h"

namespace HWP { namespace CHART
{
CMarker::CMarker()
{

}

bool CMarker::Read(CChartStream& oStream)
{
	return m_oFillColor.Read(oStream) && m_oPen.Read(oStream) &&
	       oStream.ReadSingle(m_snSize) && oStream.ReadInteger(m_nStyle) &&
	       oStream.ReadBoolean(m_bVisible) && m_oVtPicture.Read(oStream);
}
}}
