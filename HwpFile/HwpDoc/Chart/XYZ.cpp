#include "XYZ.h"

namespace HWP { namespace CHART
{
CXYZ::CXYZ()
{

}

bool CXYZ::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAutomatic) && oStream.ReadDouble(m_dxIntersection) &&
	       oStream.ReadDouble(m_dyIntersection) && oStream.ReadDouble(m_dzIntersection);
}
}}
