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
	int m_nActiveSeriesCount;
	bool m_bAllowDithering;
	bool m_bAllowDynamicRotation;
	bool m_bAllowSelections;
	bool m_bAllowSeriesSelection;
	bool m_bAllowUserChanges;
	bool m_bAutoIncrement;
	CBackdrop m_oBackdrop;
	bool m_bChart3d;
	int m_nChartType;
	int m_nColumn;
	int m_nColumnCount;
	HWP_STRING m_sColumnLabel;
	int m_nColumnLabelCount;
	int m_nColumnLabelIndex;
	HWP_STRING m_sData;
	CDataGrid m_oDataGrid;
	bool m_bDoSetCursor;
	int m_nDrawMode;
	int m_nErrorOffset;
	HWP_STRING m_sFileName;
	CFootnote m_oFootnote;
	HWP_STRING m_sFootnoteText;
	long m_lHandle;
	CLegend m_oLegend;
	int m_nPicture;
	CPlot m_oPlot;
	CPrintInformation m_oPrintInformation;
	bool m_bRandomFill;
	bool m_bRepaint;
	int m_nRow;
	int m_nRowCount;
	HWP_STRING m_sRowLabel;
	int m_nRowLabelCount;
	int m_nRowLabelIndex;
	int m_nSeriesColumn;
	int m_nSeriesType;
	bool m_bShowLegend;
	int m_nSsLinkMode;
	HWP_STRING m_sSsLinkRange;
	HWP_STRING m_sSsLinkBook;
	bool m_bStacking;
	int m_nTextLengthType;
	CTitle m_oTitle;
	HWP_STRING m_sTitleText;
	int m_nTwipsWidth;
	int m_nTwipsHeight;
public:
	CVtChart();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // VTCHART_H
