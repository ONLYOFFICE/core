#include "Coor.h"

namespace HWP { namespace CHART
{
CCoor::CCoor()
{

}

bool CCoor::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snX) && oStream.ReadSingle(m_snY);
}
}}
