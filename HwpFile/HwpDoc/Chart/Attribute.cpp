#include "Attribute.h"

namespace HWP { namespace CHART
{
CAttribute::CAttribute()
{

}

bool CAttribute::Read(CChartStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oPen.Read(oStream) &&
	       oStream.ReadString(m_sText) && oStream.ReadDouble(m_dValue);
}
}}
