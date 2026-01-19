#include "Plot.h"

namespace HWP { namespace CHART
{
CPlotBase::CPlotBase()
{

}

bool CPlotBase::Read(CChartStream& oStream)
{
	return m_oBrush.Read(oStream) && oStream.ReadSingle(m_snBaseHeight) && m_oPen.Read(oStream);
}

CPlot::CPlot()
{

}

bool CPlot::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nAngleUnit) && oStream.ReadBoolean(m_bAutoLayout) &&
	       m_oAxis.Read(oStream) && m_oBackdrop.Read(oStream) &&
	       oStream.ReadSingle(m_snBarGap) && oStream.ReadBoolean(m_bClockwise) &&
	       oStream.ReadBoolean(m_bDataSeriesInRow) && oStream.ReadInteger(m_nDefaultPercentBasis) &&
	       oStream.ReadSingle(m_snDepthToHeightRatio) && m_oDoughnut.Read(oStream) &&
	       m_oElevation.Read(oStream) && m_oLight.Read(oStream) &&
	       m_oLocationRect.Read(oStream) && oStream.ReadSingle(m_snMaxBubbleToAxisRatio) &&
	       m_oPerspective.Read(oStream) && m_oPie.Read(oStream) &&
	       m_oPlotBase.Read(oStream) && oStream.ReadInteger(m_nProjection) &&
	       oStream.ReadSingle(m_snScaleAngle) && m_oSeries.Read(oStream) &&
	       oStream.ReadInteger(m_nSort) && oStream.ReadSingle(m_snStartingAngle) &&
	       oStream.ReadInteger(m_nSubPlotLabelPosition) && oStream.ReadBoolean(m_bUniformAxis) &&
	       m_oView3D.Read(oStream) && m_oWall.Read(oStream) &&
	       oStream.ReadSingle(m_snWidthToHeightRatio) && m_oWeighting.Read(oStream) &&
	       oStream.ReadSingle(m_snxGap) && m_oXYZ.Read(oStream) &&
	       oStream.ReadSingle(m_snzGap);
}
}}
