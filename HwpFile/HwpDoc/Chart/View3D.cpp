#include "View3D.h"

namespace HWP { namespace CHART
{
CView3D::CView3D()
{

}

bool CView3D::Read(CChartStream& oStream)
{
	return oStream.ReadSingle(m_snElevation) && oStream.ReadSingle(m_snRotation);
}
}}
