#include "Marker.h"

namespace HWP { namespace CHART
{
CMarker::CMarker()
{

}

bool CMarker::Read(CHWPStream& oStream)
{
	return m_oFillColor.Read(oStream) && m_oPen.Read(oStream) &&
	       oStream.ReadShort(m_snSize) && oStream.ReadInt(m_nStyle) &&
	       oStream.ReadBool(m_bVisible) && m_oVtPicture.Read(oStream);
}
}}
