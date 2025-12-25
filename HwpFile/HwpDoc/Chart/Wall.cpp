#include "Wall.h"

namespace HWP { namespace CHART
{
CWall::CWall()
{

}

bool CWall::Read(CHWPStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oPen.Read(oStream) && oStream.ReadShort(m_snWidth);
}
}}
