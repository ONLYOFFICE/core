#include "Bar.h"

namespace HWP { namespace CHART
{
CBar::CBar()
{

}

bool CBar::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nSides) && oStream.ReadShort(m_snTopRatio);
}
}}
