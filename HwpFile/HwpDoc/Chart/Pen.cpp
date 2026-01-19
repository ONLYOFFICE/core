#include "Pen.h"

namespace HWP { namespace CHART
{
CPen::CPen()
{

}

bool CPen::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nCap) && oStream.ReadInteger(m_nJoin) &&
	       oStream.ReadSingle(m_snLimit) && oStream.ReadInteger(m_nStyle) &&
	       oStream.ReadSingle(m_snWidth) && m_oVtColor.Read(oStream);
}
}}
