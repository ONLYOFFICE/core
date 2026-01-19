#ifndef SERIES_H
#define SERIES_H

#include "Bar.h"
#include "DataPoint.h"
#include "Pen.h"
#include "HiLo.h"
#include "Position.h"
#include "StatLine.h"

namespace HWP { namespace CHART
{
class CSeriesLabel : public IChartObject
{
	CBackdrop m_oBackdrop;
	CHART_INTEGER m_nLineStyle;
	CHART_INTEGER m_nLocationType;
	CCoor m_oOffset;
	CHART_STRING m_sText;
	CTextLayout m_oTextLayout;
	CHART_SINGLE m_snTextLength;
	CVtFont m_oVtFont;
public:
	CSeriesLabel();

	bool Read(CChartStream& oStream) override;
};

class CSeriesMarker : public IChartObject
{
	bool m_bAuto;
	bool m_bShow;
public:
	CSeriesMarker();

	bool Read(CChartStream& oStream) override;
};

class CSeries : public IChartObject
{
	CBar m_oBar;
	CDataPoints m_oDataPoints;
	CPen m_oGuidelinePen;
	CHiLo m_oHiLo;
	CHART_STRING m_sLegendText;
	CPen m_oPen;
	CPosition m_oPosition;
	CHART_BOOLEAN m_bSecondaryAxis;
	CSeriesLabel m_oSeriesLabel;
	CSeriesMarker m_oSeriesMarker;
	CHART_INTEGER m_nSeriesType;
	CHART_BOOLEAN m_bShowGuideLines;
	CHART_BOOLEAN m_bShowLine;
	CHART_INTEGER m_nSmoothingFactor;
	CHART_INTEGER m_nSmoothingType;
	CStatLine m_oStatLine;
public:
	CSeries();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // SERIES_H
