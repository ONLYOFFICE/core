#include "TextLayout.h"

namespace HWP { namespace CHART
{
CTextLayout::CTextLayout()
{

}

bool CTextLayout::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bWordWrap) && oStream.ReadInt(m_nHorzAlignment) &&
	       oStream.ReadInt(m_nOrientation) && oStream.ReadInt(m_nVertAlignment);
}
}}
