#include "Series.h"

namespace HWP { namespace CHART
{
CSeriesLabel::CSeriesLabel()
{

}

bool CSeriesLabel::Read(CChartStream& oStream)
{
	return m_oBackdrop.Read(oStream) && oStream.ReadInteger(m_nLineStyle) &&
	       oStream.ReadInteger(m_nLocationType) && m_oOffset.Read(oStream) &&
	       oStream.ReadString(m_sText) && m_oTextLayout.Read(oStream) &&
	       oStream.ReadSingle(m_snTextLength) && m_oVtFont.Read(oStream);
}

CSeriesMarker::CSeriesMarker()
{

}

bool CSeriesMarker::Read(CChartStream& oStream)
{
	return oStream.ReadBoolean(m_bAuto) && oStream.ReadBoolean(m_bShow);
}

CSeries::CSeries()
{

}

bool CSeries::Read(CChartStream& oStream)
{
	return m_oBar.Read(oStream) && m_oDataPoints.Read(oStream) &&
	       m_oGuidelinePen.Read(oStream) && m_oHiLo.Read(oStream) &&
	       oStream.ReadString(m_sLegendText) && m_oPen.Read(oStream) &&
	       m_oPosition.Read(oStream) && oStream.ReadBoolean(m_bSecondaryAxis) &&
	       m_oSeriesLabel.Read(oStream) && m_oSeriesMarker.Read(oStream) &&
	       oStream.ReadInteger(m_nSeriesType) && oStream.ReadBoolean(m_bShowGuideLines) &&
	       oStream.ReadBoolean(m_bShowLine) && oStream.ReadInteger(m_nSmoothingFactor) &&
	       oStream.ReadInteger(m_nSmoothingType) && m_oStatLine.Read(oStream);
}
}}
