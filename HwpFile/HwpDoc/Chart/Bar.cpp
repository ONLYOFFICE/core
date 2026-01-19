#include "Bar.h"

namespace HWP { namespace CHART
{
CBar::CBar()
{

}

bool CBar::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nSides) && oStream.ReadSingle(m_snTopRatio);
}
}}
