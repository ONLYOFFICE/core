#include "DateScale.h"

namespace HWP { namespace CHART
{
CDateScale::CDateScale()
{

}

bool CDateScale::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAuto) && oStream.ReadInteger(m_nMajFreq) &&
	       oStream.ReadInteger(m_nMajInt) && oStream.ReadDouble(m_dMaximum) &&
	       oStream.ReadDouble(m_dMinimum) && oStream.ReadInteger(m_nMinFreq) &&
	       oStream.ReadInteger(m_nMinInt) && oStream.ReadBoolean(m_bSkipWeekend);
}
}}
