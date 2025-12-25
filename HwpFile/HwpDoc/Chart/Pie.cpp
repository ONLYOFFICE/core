#include "Pie.h"

namespace HWP { namespace CHART
{
CPie::CPie()
{

}

bool CPie::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snThicknessRatio) && oStream.ReadShort(m_snTopRadiusRatio);
}
}}
