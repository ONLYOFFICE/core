#include "CategoryScale.h"

namespace HWP { namespace CHART
{
CCategoryScale::CCategoryScale()
{

}

bool CCategoryScale::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAuto) && oStream.ReadInteger(m_nDivisionsPerLabel) &&
	       oStream.ReadInteger(m_nDivisionsPerTick) && oStream.ReadBoolean(m_bLabelTick);
}
}}
