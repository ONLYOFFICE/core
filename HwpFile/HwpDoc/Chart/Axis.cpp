#include "Axis.h"

namespace HWP { namespace CHART
{
CAxisGrid::CAxisGrid()
{

}

bool CAxisGrid::Read(CChartStream& oStream)
{
	return m_oMajorPen.Read(oStream) && m_oMinorPen.Read(oStream);
}

CAxisScale::CAxisScale()
{

}

bool CAxisScale::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bHide) && oStream.ReadInteger(m_nLogBase) &&
	       oStream.ReadString(m_sPercentBasis) && oStream.ReadInteger(m_nType);
}

CAxisTitle::CAxisTitle()
{

}

bool CAxisTitle::Read(CChartStream& oStream)
{
	return m_oBackdrop.Read(oStream) && oStream.ReadString(m_sText) &&
	       m_oTextLayout.Read(oStream) && oStream.ReadInteger(m_nTextLength) &&
	       oStream.ReadBoolean(m_bVisible) && m_oVtFont.Read(oStream);
}

CAxis::CAxis()
{

}

bool CAxis::Read(CChartStream& oStream)
{
	return m_oAxisGrid.Read(oStream) && m_oAxisScale.Read(oStream) &&
	       m_oAxisTitle.Read(oStream) && m_oCategoryScale.Read(oStream) &&
	       m_oDateScale.Read(oStream) && m_oIntersection.Read(oStream) &&
	       m_oLabels.Read(oStream) && oStream.ReadInteger(m_nLabelLevelCount) &&
	       m_oPen.Read(oStream) && m_oTick.Read(oStream) && m_oValueScale.Read(oStream);
}
}}
