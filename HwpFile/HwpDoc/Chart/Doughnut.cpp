#include "Doughnut.h"

namespace HWP { namespace CHART
{
CDoughnut::CDoughnut()
{

}

bool CDoughnut::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nSides) && oStream.ReadShort(m_snInteriorRatio);
}
}}
