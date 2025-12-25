#include "Elevation.h"

namespace HWP { namespace CHART
{
CElevation::CElevation()
{

}

bool CElevation::Read(CHWPStream& oStream)
{
	return m_oAttributes.Read(oStream) && oStream.ReadBool(m_bAutoValues) &&
	       oStream.ReadInt(m_nColorType) && oStream.ReadInt(m_nColSmoothing) &&
	       m_oContour.Read(oStream) && m_oContourGradient.Read(oStream) &&
	       oStream.ReadInt(m_nRowSmoothing) && oStream.ReadBool(m_bSeparateContourData) &&
	       m_oSurface.Read(oStream);
}
}}
