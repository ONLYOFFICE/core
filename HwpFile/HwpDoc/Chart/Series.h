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
class CSeriesLabel
{
	CBackdrop m_oBackdrop;
	int m_nLineStyle;
	int m_nLocationType;
	CCoor m_oOffset;
	HWP_STRING m_sText;
	CTextLayout m_oTextLayout;
	SINGLE m_snTextLength;
	CVtFont m_oVtFont;
public:
	CSeriesLabel();
};

class CSeriesMarker
{
	bool m_bAuto;
	bool m_bShow;
public:
	CSeriesMarker();
};

class CSeries
{
	CBar m_oBar;
	CDataPoints m_oDataPoints;
	CPen m_oGuidelinePen;
	CHiLo m_oHiLo;
	HWP_STRING m_sLegendText;
	CPen m_oPen;
	CPosition m_oPosition;
	bool m_bSecondaryAxis;
	CSeriesLabel m_oSeriesLabel;
	CSeriesMarker m_oSeriesMarker;
	int m_nSeriesType;
	bool m_bShowGuideLines;
	bool m_bShowLine;
	int m_nSmoothingFactor;
	int m_nSmoothingType;
	CStatLine m_oStatLine;
public:
	CSeries();
};
}}

#endif // SERIES_H
