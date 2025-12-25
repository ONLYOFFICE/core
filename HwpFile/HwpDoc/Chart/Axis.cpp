#include "Axis.h"

namespace HWP { namespace CHART
{
CAxisGrid::CAxisGrid()
{

}

bool CAxisGrid::Read(CHWPStream& oStream)
{
	return m_oMajorPen.Read(oStream) && m_oMinorPen.Read(oStream);
}

CAxisScale::CAxisScale()
{

}

bool CAxisScale::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bHide) && oStream.ReadInt(m_nLogBase) &&
	       oStream.ReadString(m_sPercentBasis, DEFAULT_STRING_CHARACTER) && oStream.ReadInt(m_nType);
}

CAxisTitle::CAxisTitle()
{

}

bool CAxisTitle::Read(CHWPStream& oStream)
{
	return m_oBackdrop.Read(oStream) && oStream.ReadString(m_sText, DEFAULT_STRING_CHARACTER) &&
	       m_oTextLayout.Read(oStream) && oStream.ReadInt(m_nTextLength) &&
	       oStream.ReadBool(m_bVisible) && m_oVtFont.Read(oStream);
}

CAxis::CAxis()
{

}

bool CAxis::Read(CHWPStream& oStream)
{
	return m_oAxisGrid.Read(oStream) && m_oAxisScale.Read(oStream) &&
	       m_oAxisTitle.Read(oStream) && m_oCategoryScale.Read(oStream) &&
	       m_oDateScale.Read(oStream) && m_oIntersection.Read(oStream) &&
	       m_oLabels.Read(oStream) && oStream.ReadInt(m_nLabelLevelCount) &&
	       m_oPen.Read(oStream) && m_oTick.Read(oStream) && m_oValueScale.Read(oStream);
}
}}
