#include "Coor.h"

namespace HWP { namespace CHART
{
CCoor::CCoor()
{

}

bool CCoor::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snX) && oStream.ReadShort(m_snY);
}
}}
