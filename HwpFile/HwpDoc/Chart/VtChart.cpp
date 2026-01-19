#include "VtChart.h"

namespace HWP { namespace CHART
{
CVtChart::CVtChart()
{

}

bool CVtChart::Read(CChartStream& oStream)
{
	return oStream.ReadInteger(m_nActiveSeriesCount) && oStream.ReadBoolean(m_bAllowDithering) &&
	       oStream.ReadBoolean(m_bAllowDynamicRotation) && oStream.ReadBoolean(m_bAllowSelections) &&
	       oStream.ReadBoolean(m_bAllowSeriesSelection) && oStream.ReadBoolean(m_bAllowUserChanges) &&
	       oStream.ReadBoolean(m_bAutoIncrement) && m_oBackdrop.Read(oStream) &&
	       oStream.ReadBoolean(m_bChart3d) && oStream.ReadInteger(m_nChartType) &&
	       oStream.ReadInteger(m_nColumn) && oStream.ReadInteger(m_nColumnCount) &&
	       oStream.ReadString(m_sColumnLabel) && oStream.ReadInteger(m_nColumnLabelCount)
	       && oStream.ReadInteger(m_nColumnLabelIndex) && oStream.ReadString(m_sData) &&
	       m_oDataGrid.Read(oStream) && oStream.ReadBoolean(m_bDoSetCursor) &&
	       oStream.ReadInteger(m_nDrawMode) && oStream.ReadInteger(m_nErrorOffset) &&
	       oStream.ReadString(m_sFileName) && m_oFootnote.Read(oStream) &&
	       oStream.ReadString(m_sFootnoteText) && oStream.ReadInteger(m_nPicture) &&
	       m_oPlot.Read(oStream) && m_oPrintInformation.Read(oStream) &&
	       oStream.ReadBoolean(m_bRandomFill) && oStream.ReadBoolean(m_bRepaint) &&
	       oStream.ReadInteger(m_nRow) && oStream.ReadInteger(m_nRowCount) &&
	       oStream.ReadString(m_sRowLabel) && oStream.ReadInteger(m_nRowLabelCount) &&
	       oStream.ReadInteger(m_nRowLabelIndex) && oStream.ReadInteger(m_nSeriesColumn) &&
	       oStream.ReadInteger(m_nSeriesType) && oStream.ReadBoolean(m_bShowLegend) &&
	       oStream.ReadInteger(m_nSsLinkMode) && oStream.ReadString(m_sSsLinkRange) &&
	       oStream.ReadString(m_sSsLinkBook) && oStream.ReadBoolean(m_bStacking) &&
	       oStream.ReadInteger(m_nTextLengthType) && m_oTitle.Read(oStream) &&
	       oStream.ReadString(m_sTitleText) && oStream.ReadInteger(m_nTwipsWidth) &&
	       oStream.ReadInteger(m_nTwipsHeight);
}
}}
