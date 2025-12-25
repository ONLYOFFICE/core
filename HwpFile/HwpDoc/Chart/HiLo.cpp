#include "HiLo.h"

namespace HWP { namespace CHART
{
CHiLo::CHiLo()
{

}

bool CHiLo::Read(CHWPStream& oStream)
{
	return m_oGainColor.Read(oStream) && m_oLossColor.Read(oStream);
}
}}
