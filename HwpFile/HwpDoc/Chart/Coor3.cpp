#include "Coor3.h"

namespace HWP { namespace CHART
{
CCoor3::CCoor3()
{

}

bool CCoor3::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snX) && oStream.ReadSingle(m_snY) && oStream.ReadSingle(m_snZ);
}
}}
