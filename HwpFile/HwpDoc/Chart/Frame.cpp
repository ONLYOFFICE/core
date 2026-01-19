#include "Frame.h"

namespace HWP { namespace CHART
{
CFrame::CFrame()
{

}

bool CFrame::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nStyle) && oStream.ReadSingle(m_snWidth) &&
	       m_oFrameColor.Read(oStream) && m_oSpaceColor.Read(oStream);
}
}}
