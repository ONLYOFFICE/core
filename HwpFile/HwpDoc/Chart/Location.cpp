#include "Location.h"

namespace HWP { namespace CHART
{
CLocation::CLocation()
{

}

bool CLocation::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nLocationType) && m_oRect.Read(oStream) && oStream.ReadBool(m_bVisible);
}
}}
