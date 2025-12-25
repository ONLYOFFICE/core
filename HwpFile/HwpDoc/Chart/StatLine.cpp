#include "StatLine.h"

namespace HWP { namespace CHART
{
CStatLine::CStatLine()
{

}

bool CStatLine::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nFlags) && oStream.ReadInt(m_nStyle) &&
	       m_oVtColor.Read(oStream) && oStream.ReadShort(m_snWidth);
}
}}
