#include "Pen.h"

namespace HWP { namespace CHART
{
CPen::CPen()
{

}

bool CPen::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nCap) && oStream.ReadInt(m_nJoin) &&
	       oStream.ReadShort(m_snLimit) && oStream.ReadInt(m_nStyle) &&
	       oStream.ReadShort(m_snWidth) && m_oVtColor.Read(oStream);
}
}}
