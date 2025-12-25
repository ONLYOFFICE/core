#include "VtFont.h"

namespace HWP { namespace CHART
{
CVtFont::CVtFont()
{

}

bool CVtFont::Read(CHWPStream& oStream)
{
	return m_oColor.Read(oStream) && oStream.ReadInt(m_nEffects) &&
	       oStream.ReadString(m_sName, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadShort(m_snSize) && oStream.ReadInt(m_nStyle);
}
}}
