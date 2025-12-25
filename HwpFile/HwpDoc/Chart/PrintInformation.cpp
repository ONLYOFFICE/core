#include "PrintInformation.h"

namespace HWP { namespace CHART
{
CPrintInformation::CPrintInformation()
{

}

bool CPrintInformation::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snBottomMargin) && oStream.ReadBool(m_bCenterHorizontally) &&
	       oStream.ReadBool(m_bCenterVertically) && oStream.ReadBool(m_bLayoutForPrinter) &&
	       oStream.ReadShort(m_snLeftMargin) && oStream.ReadBool(m_bMonochrome) &&
	       oStream.ReadInt(m_nOrientation) && oStream.ReadShort(m_snRightMargin) &&
	       oStream.ReadInt(m_nScaleType) && oStream.ReadShort(m_snTopMargin);
}
}}
