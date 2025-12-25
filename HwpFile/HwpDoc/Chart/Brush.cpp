#include "Brush.h"

namespace HWP { namespace CHART
{
CBrush::CBrush()
{

}

bool CBrush::Read(CHWPStream& oStream)
{
	return m_oFillColor.Read(oStream) && oStream.ReadInt(m_nIndex) &&
	       m_oPatternColor.Read(oStream) && oStream.ReadInt(m_nStyle);
}
}}
