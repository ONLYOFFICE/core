#include "VtColor.h"

namespace HWP { namespace CHART
{
CVtColor::CVtColor()
{

}

bool CVtColor::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAutomatic) && oStream.ReadInt(m_nBlue) &&
	       oStream.ReadInt(m_nGreen) && oStream.ReadInt(m_nRed) && oStream.ReadInt(m_nValue);
}
}}
