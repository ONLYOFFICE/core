#include "Weighting.h"

namespace HWP { namespace CHART
{
CWeighting::CWeighting()
{

}

bool CWeighting::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nBasis) && oStream.ReadInteger(m_nStyle);
}
}}
