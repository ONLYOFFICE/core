#include "VtPicture.h"

namespace HWP { namespace CHART
{
CVtPicture::CVtPicture()
{

}

bool CVtPicture::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bEmbedded) && oStream.ReadString(m_sFilename, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadInt(m_nMap) && oStream.ReadInt(m_nType);
}

}}
