#include "Label.h"

namespace HWP { namespace CHART
{
CLabel::CLabel()
{

}

bool CLabel::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAuto) && m_oBackdrop.Read(oStream) &&
	       oStream.ReadString(m_sFormat) && oStream.ReadString(m_sFormatLength) &&
	       oStream.ReadBoolean(m_bStanding) && m_oTextLayout.Read(oStream) &&
	       m_oVtFont.Read(oStream);
}
}}
