#include "Title.h"

namespace HWP { namespace CHART
{
CTitle::CTitle()
{

}

bool CTitle::Read(CChartStream& oStream)
{
	return m_oBackdrop.Read(oStream) && m_oLocation.Read(oStream) &&
	       oStream.ReadString(m_sText) && m_oTextLayout.Read(oStream)
	       && oStream.ReadInteger(m_nTextLength) && m_oVtFont.Read(oStream);
}
}}
