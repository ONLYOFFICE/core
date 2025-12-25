#include "Backdrop.h"

namespace HWP { namespace CHART
{
CBackdrop::CBackdrop()
{

}

bool CBackdrop::Read(CHWPStream& oStream)
{
	return m_oFrame.Read(oStream) && m_oFill.Read(oStream) && m_oShadow.Read(oStream);
}
}}
