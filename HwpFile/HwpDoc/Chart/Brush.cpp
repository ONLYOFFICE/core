#include "Brush.h"

namespace HWP { namespace CHART
{
CBrush::CBrush()
{

}

bool CBrush::Read(CChartStream& oStream)
{
	return m_oFillColor.Read(oStream) && oStream.ReadInteger(m_nIndex) &&
	       m_oPatternColor.Read(oStream) && oStream.ReadInteger(m_nStyle);
}
}}
