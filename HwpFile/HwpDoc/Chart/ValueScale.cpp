#include "ValueScale.h"

namespace HWP { namespace CHART
{
CValueScale::CValueScale()
{

}

bool CValueScale::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAuto) && oStream.ReadInt(m_nMajorDivision) &&
	       oStream.ReadDouble(m_dMaximum) && oStream.ReadDouble(m_dMinimum) &&
	       oStream.ReadInt(m_nMinorDivision);
}
}}
