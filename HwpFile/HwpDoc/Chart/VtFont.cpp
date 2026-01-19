#include "VtFont.h"

namespace HWP { namespace CHART
{
CVtFont::CVtFont()
{

}

bool CVtFont::Read(CChartStream& oStream)
{
	return m_oColor.Read(oStream) && oStream.ReadInteger(m_nEffects) &&
	       oStream.ReadString(m_sName) && oStream.ReadSingle(m_snSize) &&
	       oStream.ReadInteger(m_nStyle);
}
}}
