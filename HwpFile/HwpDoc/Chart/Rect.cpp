#include "Rect.h"

namespace HWP { namespace CHART
{
CRect::CRect()
{

}

bool CRect::Read(CChartStream& oStream)
{
	return m_oMin.Read(oStream) && m_oMax.Read(oStream);
}
}}
