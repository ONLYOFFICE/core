#include "Intersection.h"

namespace HWP { namespace CHART
{
CIntersection::CIntersection()
{

}

bool CIntersection::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAuto) && oStream.ReadInteger(m_nAxisId) &&
	       oStream.ReadInteger(m_nIndex) && oStream.ReadBoolean(m_bLabelsInsidePlot) &&
	       oStream.ReadDouble(m_dPoint);
}
}}
