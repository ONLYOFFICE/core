#include "VtPicture.h"

namespace HWP { namespace CHART
{
CVtPicture::CVtPicture()
{

}

bool CVtPicture::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bEmbedded) && oStream.ReadString(m_sFilename) &&
	       oStream.ReadInteger(m_nMap) && oStream.ReadInteger(m_nType);
}

}}
