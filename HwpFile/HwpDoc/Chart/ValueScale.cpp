#include "ValueScale.h"

namespace HWP { namespace CHART
{
CValueScale::CValueScale()
{

}

bool CValueScale::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAuto) && oStream.ReadInteger(m_nMajorDivision) &&
	       oStream.ReadDouble(m_dMaximum) && oStream.ReadDouble(m_dMinimum) &&
	       oStream.ReadInteger(m_nMinorDivision);
}
}}
