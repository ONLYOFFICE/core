#include "Rect.h"

namespace HWP { namespace CHART
{
CRect::CRect()
{

}

bool CRect::Read(CHWPStream& oStream)
{
	return m_oMin.Read(oStream) && m_oMax.Read(oStream);
}
}}
