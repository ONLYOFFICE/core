#include "Coor3.h"

namespace HWP { namespace CHART
{
CCoor3::CCoor3()
{

}

bool CCoor3::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snX) && oStream.ReadShort(m_snY) && oStream.ReadShort(m_snZ);
}
}}
