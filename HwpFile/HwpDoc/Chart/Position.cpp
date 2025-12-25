#include "Position.h"

namespace HWP { namespace CHART
{
CPosition::CPosition()
{

}

bool CPosition::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bExcluded) && oStream.ReadBool(m_bHidden) &&
	       oStream.ReadInt(m_nOrder) && oStream.ReadInt(m_nStackOrder);
}
}}
