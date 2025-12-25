#include "Contour.h"

namespace HWP { namespace CHART
{
CContour::CContour()
{

}

bool CContour::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nDisplayType);
}
}}
