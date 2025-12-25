#include "LCoor.h"

namespace HWP { namespace CHART
{
CLCoor::CLCoor()
{

}

bool CLCoor::Read(CHWPStream& oStream)
{
	return oStream.ReadLong(m_lX) && oStream.ReadLong(m_lY);
}
}}
