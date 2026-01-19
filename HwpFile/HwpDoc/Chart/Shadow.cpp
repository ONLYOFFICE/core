#include "Shadow.h"

namespace HWP { namespace CHART
{
CShadow::CShadow()
{

}

bool CShadow::Read(CChartStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oOffset.Read(oStream) && oStream.ReadInteger(m_nStyle);
}
}}
