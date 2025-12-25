#include "XYZ.h"

namespace HWP { namespace CHART
{
CXYZ::CXYZ()
{

}

bool CXYZ::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAutomatic) && oStream.ReadDouble(m_dxIntersection) &&
	       oStream.ReadDouble(m_dyIntersection) && oStream.ReadDouble(m_dzIntersection);
}
}}
