#include "Pie.h"

namespace HWP { namespace CHART
{
CPie::CPie()
{

}

bool CPie::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snThicknessRatio) && oStream.ReadSingle(m_snTopRadiusRatio);
}
}}
