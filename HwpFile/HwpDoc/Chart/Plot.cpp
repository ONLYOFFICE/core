#include "Plot.h"

namespace HWP { namespace CHART
{
CPlotBase::CPlotBase()
{

}

bool CPlotBase::Read(CHWPStream& oStream)
{
	return m_oBrush.Read(oStream) && oStream.ReadShort(m_snBaseHeight) && m_oPen.Read(oStream);
}

CPlot::CPlot()
{

}

bool CPlot::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nAngleUnit) && oStream.ReadBool(m_bAutoLayout) &&
	       m_oAxis.Read(oStream) && m_oBackdrop.Read(oStream) &&
	       oStream.ReadShort(m_snBarGap) && oStream.ReadBool(m_bClockwise) &&
	       oStream.ReadBool(m_bDataSeriesInRow) && oStream.ReadInt(m_nDefaultPercentBasis) &&
	       oStream.ReadShort(m_snDepthToHeightRatio) && m_oDoughnut.Read(oStream) &&
	       m_oElevation.Read(oStream) && m_oLight.Read(oStream) &&
	       m_oLocationRect.Read(oStream) && oStream.ReadShort(m_snMaxBubbleToAxisRatio) &&
	       m_oPerspective.Read(oStream) && m_oPie.Read(oStream) &&
	       m_oPlotBase.Read(oStream) && oStream.ReadInt(m_nProjection) &&
	       oStream.ReadShort(m_snScaleAngle) && m_oSeries.Read(oStream) &&
	       oStream.ReadInt(m_nSort) && oStream.ReadShort(m_snStartingAngle) &&
	       oStream.ReadInt(m_nSubPlotLabelPosition) && oStream.ReadBool(m_bUniformAxis) &&
	       m_oView3D.Read(oStream) && m_oWall.Read(oStream) &&
	       oStream.ReadShort(m_snWidthToHeightRatio) && m_oWeighting.Read(oStream) &&
	       oStream.ReadShort(m_snxGap) && m_oXYZ.Read(oStream) &&
	       oStream.ReadShort(m_snzGap);
}
}}
