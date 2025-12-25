#include "View3D.h"

namespace HWP { namespace CHART
{
CView3D::CView3D()
{

}

bool CView3D::Read(CHWPStream& oStream)
{
	return oStream.ReadShort(m_snElevation) && oStream.ReadShort(m_snRotation);
}
}}
