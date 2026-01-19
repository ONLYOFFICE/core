#include "PrintInformation.h"

namespace HWP { namespace CHART
{
CPrintInformation::CPrintInformation()
{

}

bool CPrintInformation::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snBottomMargin) && oStream.ReadBoolean(m_bCenterHorizontally) &&
	       oStream.ReadBoolean(m_bCenterVertically) && oStream.ReadBoolean(m_bLayoutForPrinter) &&
	       oStream.ReadSingle(m_snLeftMargin) && oStream.ReadBoolean(m_bMonochrome) &&
	       oStream.ReadInteger(m_nOrientation) && oStream.ReadSingle(m_snRightMargin) &&
	       oStream.ReadInteger(m_nScaleType) && oStream.ReadSingle(m_snTopMargin);
}
}}
