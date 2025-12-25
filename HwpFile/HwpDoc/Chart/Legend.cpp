#include "Legend.h"

namespace HWP { namespace CHART
{
CLegend::CLegend()
{

}

bool CLegend::Read(CHWPStream& oStream)
{
	return m_oBackdrop.Read(oStream) && m_oLocation.Read(oStream) &&
	       m_oTextLayout.Read(oStream) && m_oVtFont.Read(oStream);
}
}}
