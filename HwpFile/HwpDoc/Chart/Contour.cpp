#include "Contour.h"

namespace HWP { namespace CHART
{
CContour::CContour()
{

}

bool CContour::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nDisplayType);
}
}}
