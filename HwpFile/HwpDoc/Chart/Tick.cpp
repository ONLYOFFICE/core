#include "Tick.h"

namespace HWP { namespace CHART
{
CTick::CTick()
{

}

bool CTick::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snLength) && oStream.ReadInteger(m_nStyle);
}
}}
