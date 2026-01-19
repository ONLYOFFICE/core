#include "StatLine.h"

namespace HWP { namespace CHART
{
CStatLine::CStatLine()
{

}

bool CStatLine::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nFlags) && oStream.ReadInteger(m_nStyle) &&
	       m_oVtColor.Read(oStream) && oStream.ReadSingle(m_snWidth);
}
}}
