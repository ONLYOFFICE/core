#include "TextLayout.h"

namespace HWP { namespace CHART
{
CTextLayout::CTextLayout()
{

}

bool CTextLayout::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bWordWrap) && oStream.ReadInteger(m_nHorzAlignment) &&
	       oStream.ReadInteger(m_nOrientation) && oStream.ReadInteger(m_nVertAlignment);
}
}}
