#include "Location.h"

namespace HWP { namespace CHART
{
CLocation::CLocation()
{

}

bool CLocation::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nLocationType) && m_oRect.Read(oStream) && oStream.ReadBoolean(m_bVisible);
}
}}
