#include "Footnote.h"

namespace HWP { namespace CHART
{
CFootnote::CFootnote()
{

}

bool CFootnote::Read(CHWPStream& oStream)
{
	return m_oBackdrop.Read(oStream) && m_oLocation.Read(oStream) &&
	       oStream.ReadString(m_sText, DEFAULT_STRING_CHARACTER) &&
	       m_oTextLayout.Read(oStream) && oStream.ReadInt(m_nTextLength) &&
	       m_oVtFont.Read(oStream);
}
}}
