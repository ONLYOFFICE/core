#include "LRect.h"

namespace HWP { namespace CHART
{
CLRect::CLRect()
{

}

bool CLRect::Read(CChartStream& oStream)
{
	return m_oMax.Read(oStream) && m_oMin.Read(oStream);
}
}}
