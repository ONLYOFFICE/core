#include "Attribute.h"

namespace HWP { namespace CHART
{
CAttribute::CAttribute()
{

}

bool CAttribute::Read(CHWPStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oPen.Read(oStream) &&
	       oStream.ReadString(m_sText, DEFAULT_STRING_CHARACTER) && oStream.ReadDouble(m_dValue);
}
}}
