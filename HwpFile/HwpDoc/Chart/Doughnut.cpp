#include "Doughnut.h"

namespace HWP { namespace CHART
{
CDoughnut::CDoughnut()
{

}

bool CDoughnut::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nSides) && oStream.ReadSingle(m_snInteriorRatio);
}
}}
