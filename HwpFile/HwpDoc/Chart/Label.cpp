#include "Label.h"

namespace HWP { namespace CHART
{
CLabel::CLabel()
{

}

bool CLabel::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAuto) && m_oBackdrop.Read(oStream) &&
	       oStream.ReadString(m_sFormat, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadString(m_sFormatLength, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadBool(m_bStanding) && m_oTextLayout.Read(oStream) &&
	       m_oVtFont.Read(oStream);
}
}}
