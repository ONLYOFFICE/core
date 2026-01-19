#include "DataPoint.h"

namespace HWP { namespace CHART
{
CDataPointLabel::CDataPointLabel()
{

}

bool CDataPointLabel::Read(CChartStream& oStream)
{
	return m_oBackdrop.Read(oStream) && oStream.ReadInteger(m_nComponent) &&
	       oStream.ReadBoolean(m_bCustom) && oStream.ReadInteger(m_nLineStyle) &&
	       oStream.ReadInteger(m_nLocationType) && m_oOffset.Read(oStream) &&
	       oStream.ReadString(m_sPercentFormat) && oStream.ReadString(m_sText) &&
	       m_oTextLayout.Read(oStream) && oStream.ReadInteger(m_nTextLength) &&
	       oStream.ReadString(m_sValueFormat) && m_oVtFont.Read(oStream);
}

CDataPoint::CDataPoint()
{

}

bool CDataPoint::Read(CChartStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oDataPointLabel.Read(oStream) &&
	       m_oEdgePen.Read(oStream) && oStream.ReadSingle(m_snOffset) &&
	       m_oMarker.Read(oStream) && m_oVtPicture.Read(oStream);
}
}}
