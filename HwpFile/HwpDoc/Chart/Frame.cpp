#include "Frame.h"

namespace HWP { namespace CHART
{
CFrame::CFrame()
{

}

bool CFrame::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nStyle) && oStream.ReadShort(m_snWidth) &&
	       m_oFrameColor.Read(oStream) && m_oSpaceColor.Read(oStream);
}
}}
