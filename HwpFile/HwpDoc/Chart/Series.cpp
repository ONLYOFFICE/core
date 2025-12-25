#include "Series.h"

namespace HWP { namespace CHART
{
CSeriesLabel::CSeriesLabel()
{

}

bool CSeriesLabel::Read(CHWPStream& oStream)
{
	return m_oBackdrop.Read(oStream) && oStream.ReadInt(m_nLineStyle) &&
	       oStream.ReadInt(m_nLocationType) && m_oOffset.Read(oStream) &&
	       oStream.ReadString(m_sText, DEFAULT_STRING_CHARACTER) &&
	       m_oTextLayout.Read(oStream) && oStream.ReadShort(m_snTextLength) &&
	       m_oVtFont.Read(oStream);
}

CSeriesMarker::CSeriesMarker()
{

}

bool CSeriesMarker::Read(CHWPStream& oStream)
{
	return oStream.ReadBool(m_bAuto) && oStream.ReadBool(m_bShow);
}

CSeries::CSeries()
{

}

bool CSeries::Read(CHWPStream& oStream)
{
	return m_oBar.Read(oStream) && m_oDataPoints.Read(oStream) &&
	       m_oGuidelinePen.Read(oStream) && m_oHiLo.Read(oStream) &&
	       oStream.ReadString(m_sLegendText, DEFAULT_STRING_CHARACTER) &&
	       m_oPen.Read(oStream) && m_oPosition.Read(oStream) &&
	       oStream.ReadBool(m_bSecondaryAxis) && m_oSeriesLabel.Read(oStream) &&
	       m_oSeriesMarker.Read(oStream) && oStream.ReadInt(m_nSeriesType) &&
	       oStream.ReadBool(m_bShowGuideLines) && oStream.ReadBool(m_bShowLine) &&
	       oStream.ReadInt(m_nSmoothingFactor) && oStream.ReadInt(m_nSmoothingType) &&
	       m_oStatLine.Read(oStream);
}
}}
