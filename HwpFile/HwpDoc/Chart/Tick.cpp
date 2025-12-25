#include "Tick.h"

namespace HWP { namespace CHART
{
CTick::CTick()
{

}

bool CTick::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snLength) && oStream.ReadInt(m_nStyle);
}
}}
