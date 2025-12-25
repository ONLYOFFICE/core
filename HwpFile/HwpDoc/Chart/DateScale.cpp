#include "DateScale.h"

namespace HWP { namespace CHART
{
CDateScale::CDateScale()
{

}

bool CDateScale::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAuto) && oStream.ReadInt(m_nMajFreq) &&
	       oStream.ReadInt(m_nMajInt) && oStream.ReadDouble(m_dMaximum) &&
	       oStream.ReadDouble(m_dMinimum) && oStream.ReadInt(m_nMinFreq) &&
	       oStream.ReadInt(m_nMinInt) && oStream.ReadBool(m_bSkipWeekend);
}
}}
