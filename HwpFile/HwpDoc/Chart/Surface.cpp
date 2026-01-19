#include "Surface.h"

namespace HWP { namespace CHART
{
CSurface::CSurface()
{

}

bool CSurface::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nBase) && m_oBrush.Read(oStream) &&
	       oStream.ReadInteger(m_nColWireframe) && oStream.ReadInteger(m_nDisplayType) &&
	       oStream.ReadInteger(m_nProjection) && oStream.ReadInteger(m_nRowWireframe) &&
	       m_oWireframePen.Read(oStream);
}
}}
