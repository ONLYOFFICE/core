#include "Elevation.h"

namespace HWP { namespace CHART
{
CElevation::CElevation()
{

}

bool CElevation::Read(CChartStream& oStream)
{
	return m_oAttributes.Read(oStream) && oStream.ReadBoolean(m_bAutoValues) &&
	       oStream.ReadInteger(m_nColorType) && oStream.ReadInteger(m_nColSmoothing) &&
	       m_oContour.Read(oStream) && m_oContourGradient.Read(oStream) &&
	       oStream.ReadInteger(m_nRowSmoothing) && oStream.ReadBoolean(m_bSeparateContourData) &&
	       m_oSurface.Read(oStream);
}
}}
