#include "ContourGradient.h"

namespace HWP { namespace CHART
{
CContourGradient::CContourGradient()
{

}

bool CContourGradient::Read(CHWPStream& oStream)
{
	return m_oFromBrushColor.Read(oStream) && m_oToBrushColor.Read(oStream) &&
	       m_oFromPenColor.Read(oStream) && m_oToPenColor.Read(oStream);
}
}}
