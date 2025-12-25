#include "DataPoint.h"

namespace HWP { namespace CHART
{
CDataPointLabel::CDataPointLabel()
{

}

bool CDataPointLabel::Read(CHWPStream& oStream)
{
	return m_oBackdrop.Read(oStream) && oStream.ReadInt(m_nComponent) &&
	       oStream.ReadBool(m_bCustom) && oStream.ReadInt(m_nLineStyle) &&
	       oStream.ReadInt(m_nLocationType) && m_oOffset.Read(oStream) &&
	       oStream.ReadString(m_sPercentFormat, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadString(m_sText, DEFAULT_STRING_CHARACTER) &&
	       m_oTextLayout.Read(oStream) && oStream.ReadInt(m_nTextLength) &&
	       oStream.ReadString(m_sValueFormat, DEFAULT_STRING_CHARACTER) &&
	       m_oVtFont.Read(oStream);
}

CDataPoint::CDataPoint()
{

}

bool CDataPoint::Read(CHWPStream& oStream)
{
	return m_oBrush.Read(oStream) && m_oDataPointLabel.Read(oStream) &&
	       m_oEdgePen.Read(oStream) && oStream.ReadShort(m_snOffset) &&
	       m_oMarker.Read(oStream) && m_oVtPicture.Read(oStream);
}
}}
