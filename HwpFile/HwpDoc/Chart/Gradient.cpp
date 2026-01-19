#include "Gradient.h"

namespace HWP { namespace CHART
{
CGradient::CGradient()
{

}

bool CGradient::Read(CChartStream& oStream)
{
	return m_oFromColor.Read(oStream) && oStream.ReadInteger(m_nStyle) &&
	       m_oToColor.Read(oStream);
}
}}
