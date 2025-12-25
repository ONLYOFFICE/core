#include "CategoryScale.h"

namespace HWP { namespace CHART
{
CCategoryScale::CCategoryScale()
{

}

bool CCategoryScale::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAuto) && oStream.ReadInt(m_nDivisionsPerLabel) &&
	       oStream.ReadInt(m_nDivisionsPerTick) && oStream.ReadBool(m_bLabelTick);
}
}}
