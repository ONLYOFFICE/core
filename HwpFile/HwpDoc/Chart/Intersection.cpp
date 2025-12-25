#include "Intersection.h"

namespace HWP { namespace CHART
{
CIntersection::CIntersection()
{

}

bool CIntersection::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAuto) && oStream.ReadInt(m_nAxisId) &&
	       oStream.ReadInt(m_nIndex) && oStream.ReadBool(m_bLabelsInsidePlot) &&
	       oStream.ReadDouble(m_dPoint);
}
}}
