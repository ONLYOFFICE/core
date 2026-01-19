#ifndef PLOT_H
#define PLOT_H

#include "Brush.h"
#include "Pen.h"

#include "Axis.h"
#include "Coor.h"
#include "Elevation.h"
#include "Light.h"
#include "Rect.h"
#include "Coor3.h"
#include "Pie.h"
#include "Series.h"
#include "View3D.h"
#include "Wall.h"
#include "Weighting.h"
#include "XYZ.h"

namespace HWP { namespace CHART
{
class CPlotBase : public IChartObject
{
	CBrush m_oBrush;
	CHART_SINGLE m_snBaseHeight;
	CPen m_oPen;
public:
	CPlotBase();

	bool Read(CChartStream& oStream) override;
};

class CPlot : public IChartObject
{
	CHART_INTEGER m_nAngleUnit;
	CHART_BOOLEAN m_bAutoLayout;
	CAxis m_oAxis;
	CBackdrop m_oBackdrop;
	CHART_SINGLE m_snBarGap;
	CHART_BOOLEAN m_bClockwise;
	CHART_BOOLEAN m_bDataSeriesInRow;
	CHART_INTEGER m_nDefaultPercentBasis;
	CHART_SINGLE m_snDepthToHeightRatio;
	CCoor m_oDoughnut;
	CElevation m_oElevation;
	CLight m_oLight;
	CRect m_oLocationRect;
	CHART_SINGLE m_snMaxBubbleToAxisRatio;
	CCoor3 m_oPerspective;
	CPie m_oPie;
	CPlotBase m_oPlotBase;
	CHART_INTEGER m_nProjection;
	CHART_SINGLE m_snScaleAngle;
	CSeries m_oSeries;
	CHART_INTEGER m_nSort;
	CHART_SINGLE m_snStartingAngle;
	CHART_INTEGER m_nSubPlotLabelPosition;
	CHART_BOOLEAN m_bUniformAxis;
	CView3D m_oView3D;
	CWall m_oWall;
	CHART_SINGLE m_snWidthToHeightRatio;
	CWeighting m_oWeighting;
	CHART_SINGLE m_snxGap;
	CXYZ m_oXYZ;
	CHART_SINGLE m_snzGap;
public:
	CPlot();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // PLOT_H
