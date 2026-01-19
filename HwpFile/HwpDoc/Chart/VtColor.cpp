#include "VtColor.h"

namespace HWP { namespace CHART
{
CVtColor::CVtColor()
{

}

bool CVtColor::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAutomatic) && oStream.ReadInteger(m_nBlue) &&
	       oStream.ReadInteger(m_nGreen) && oStream.ReadInteger(m_nRed) &&
	       oStream.ReadInteger(m_nValue);
}
}}
