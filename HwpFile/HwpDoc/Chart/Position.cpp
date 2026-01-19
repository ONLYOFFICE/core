#include "Position.h"

namespace HWP { namespace CHART
{
CPosition::CPosition()
{

}

bool CPosition::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bExcluded) && oStream.ReadBoolean(m_bHidden) &&
	       oStream.ReadInteger(m_nOrder) && oStream.ReadInteger(m_nStackOrder);
}
}}
