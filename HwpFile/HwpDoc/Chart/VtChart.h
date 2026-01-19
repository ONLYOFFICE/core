#ifndef VTCHART_H
#define VTCHART_H

#include "ChartObject.h"
#include "DataGrid.h"
#include "Footnote.h"
#include "Legend.h"
#include "Plot.h"
#include "PrintInformation.h"
#include "Title.h"

namespace HWP { namespace CHART
{
class CVtChart : public IChartObject
{
	CHART_INTEGER m_nActiveSeriesCount;
	CHART_BOOLEAN m_bAllowDithering;
	CHART_BOOLEAN m_bAllowDynamicRotation;
	CHART_BOOLEAN m_bAllowSelections;
	CHART_BOOLEAN m_bAllowSeriesSelection;
	CHART_BOOLEAN m_bAllowUserChanges;
	CHART_BOOLEAN m_bAutoIncrement;
	CBackdrop m_oBackdrop;
	CHART_BOOLEAN m_bChart3d;
	CHART_INTEGER m_nChartType;
	CHART_INTEGER m_nColumn;
	CHART_INTEGER m_nColumnCount;
	CHART_STRING m_sColumnLabel;
	CHART_INTEGER m_nColumnLabelCount;
	CHART_INTEGER m_nColumnLabelIndex;
	CHART_STRING m_sData;
	CDataGrid m_oDataGrid;
	CHART_BOOLEAN m_bDoSetCursor;
	CHART_INTEGER m_nDrawMode;
	CHART_INTEGER m_nErrorOffset;
	CHART_STRING m_sFileName;
	CFootnote m_oFootnote;
	CHART_STRING m_sFootnoteText;
	CHART_LONG m_lHandle;
	CLegend m_oLegend;
	CHART_INTEGER m_nPicture;
	CPlot m_oPlot;
	CPrintInformation m_oPrintInformation;
	CHART_BOOLEAN m_bRandomFill;
	CHART_BOOLEAN m_bRepaint;
	CHART_INTEGER m_nRow;
	CHART_INTEGER m_nRowCount;
	CHART_STRING m_sRowLabel;
	CHART_INTEGER m_nRowLabelCount;
	CHART_INTEGER m_nRowLabelIndex;
	CHART_INTEGER m_nSeriesColumn;
	CHART_INTEGER m_nSeriesType;
	CHART_BOOLEAN m_bShowLegend;
	CHART_INTEGER m_nSsLinkMode;
	CHART_STRING m_sSsLinkRange;
	CHART_STRING m_sSsLinkBook;
	CHART_BOOLEAN m_bStacking;
	CHART_INTEGER m_nTextLengthType;
	CTitle m_oTitle;
	CHART_STRING m_sTitleText;
	CHART_INTEGER m_nTwipsWidth;
	CHART_INTEGER m_nTwipsHeight;
public:
	CVtChart();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // VTCHART_H
