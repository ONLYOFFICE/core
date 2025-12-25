#include "Weighting.h"

namespace HWP { namespace CHART
{
CWeighting::CWeighting()
{

}

bool CWeighting::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nBasis) && oStream.ReadInt(m_nStyle);
}
}}
