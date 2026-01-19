#include "Footnote.h"

namespace HWP { namespace CHART
{
CFootnote::CFootnote()
{

}

bool CFootnote::Read(CChartStream& oStream)
{
	return m_oBackdrop.Read(oStream) && m_oLocation.Read(oStream) &&
	       oStream.ReadString(m_sText) && m_oTextLayout.Read(oStream) &&
	       oStream.ReadInteger(m_nTextLength) && m_oVtFont.Read(oStream);
}
}}
