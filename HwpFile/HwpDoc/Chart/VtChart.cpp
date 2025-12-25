#include "VtChart.h"

namespace HWP { namespace CHART
{
CVtChart::CVtChart()
{

}

bool CVtChart::Read(CHWPStream& oStream)
{
	return oStream.ReadInt(m_nActiveSeriesCount) && oStream.ReadBool(m_bAllowDithering) &&
	       oStream.ReadBool(m_bAllowDynamicRotation) && oStream.ReadBool(m_bAllowSelections) &&
	       oStream.ReadBool(m_bAllowSeriesSelection) && oStream.ReadBool(m_bAllowUserChanges) &&
	       oStream.ReadBool(m_bAutoIncrement) && m_oBackdrop.Read(oStream) &&
	       oStream.ReadBool(m_bChart3d) && oStream.ReadInt(m_nChartType) &&
	       oStream.ReadInt(m_nColumn) && oStream.ReadInt(m_nColumnCount) &&
	       oStream.ReadString(m_sColumnLabel, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadInt(m_nColumnLabelCount) && oStream.ReadInt(m_nColumnLabelIndex) &&
	       oStream.ReadString(m_sData, DEFAULT_STRING_CHARACTER) &&
	       m_oDataGrid.Read(oStream) && oStream.ReadBool(m_bDoSetCursor) &&
	       oStream.ReadInt(m_nDrawMode) && oStream.ReadInt(m_nErrorOffset) &&
	       oStream.ReadString(m_sFileName, DEFAULT_STRING_CHARACTER) &&
	       m_oFootnote.Read(oStream) && oStream.ReadString(m_sFootnoteText, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadLong(m_lHandle) && m_oLegend.Read(oStream) &&
	       oStream.ReadInt(m_nPicture) && m_oPlot.Read(oStream) &&
	       m_oPrintInformation.Read(oStream) && oStream.ReadBool(m_bRandomFill) &&
	       oStream.ReadBool(m_bRepaint) && oStream.ReadInt(m_nRow) &&
	       oStream.ReadInt(m_nRowCount) && oStream.ReadString(m_sRowLabel, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadInt(m_nRowLabelCount) && oStream.ReadInt(m_nRowLabelIndex) &&
	       oStream.ReadInt(m_nSeriesColumn) && oStream.ReadInt(m_nSeriesType) &&
	       oStream.ReadBool(m_bShowLegend) && oStream.ReadInt(m_nSsLinkMode) &&
	       oStream.ReadString(m_sSsLinkRange, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadString(m_sSsLinkBook, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadBool(m_bStacking) && oStream.ReadInt(m_nTextLengthType) &&
	       m_oTitle.Read(oStream) && oStream.ReadString(m_sTitleText, DEFAULT_STRING_CHARACTER) &&
	       oStream.ReadInt(m_nTwipsWidth) && oStream.ReadInt(m_nTwipsHeight);
}
}}
