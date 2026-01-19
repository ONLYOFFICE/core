#include "LCoor.h"

namespace HWP { namespace CHART
{
CLCoor::CLCoor()
{

}

bool CLCoor::Read(CChartStream& oStream)
{
	return oStream.ReadLong(m_lX) && oStream.ReadLong(m_lY);
}
}}
