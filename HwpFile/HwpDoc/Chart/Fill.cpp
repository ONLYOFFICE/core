#include "Fill.h"

namespace HWP { namespace CHART
{
CFill::CFill()
{

}

bool CFill::Read(CHWPStream& oStream)
{
	return m_oBrush.Read(oStream) && /*&&*/
	       oStream.ReadInt(m_nStyle) && m_oVtPicture.Read(oStream);
}
}}
