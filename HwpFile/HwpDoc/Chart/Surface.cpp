#include "Surface.h"

namespace HWP { namespace CHART
{
CSurface::CSurface()
{

}

bool CSurface::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nBase) && m_oBrush.Read(oStream) &&
	       oStream.ReadInt(m_nColWireframe) && oStream.ReadInt(m_nDisplayType) &&
	       oStream.ReadInt(m_nProjection) && oStream.ReadInt(m_nRowWireframe) &&
	       m_oWireframePen.Read(oStream);
}
}}
