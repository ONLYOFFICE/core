#include "Wall.h"

namespace HWP { namespace CHART
{
CWall::CWall()
{

}

bool CWall::Read(CChartStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oPen.Read(oStream) && oStream.ReadSingle(m_snWidth);
}
}}
