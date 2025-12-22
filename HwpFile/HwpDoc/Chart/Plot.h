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
class CPlotBase
{
	CBrush m_oBrush;
	SINGLE m_snBaseHeight;
	CPen m_oPen;
public:
	CPlotBase();
};

class CPlot
{
	int m_nAngleUnit;
	bool m_bAutoLayout;
	CAxis m_oAxis;
	CBackdrop m_oBackdrop;
	SINGLE m_snBarGap;
	bool m_bClockwise;
	bool m_bDataSeriesInRow;
	int m_nDefaultPercentBasis;
	SINGLE m_snDepthToHeightRatio;
	CCoor m_oDoughnut;
	CElevation m_oElevation;
	CLight m_oLight;
	CRect m_oLocationRect;
	SINGLE m_snMaxBubbleToAxisRatio;
	CCoor3 m_oPerspective;
	CPie m_oPie;
	CPlotBase m_oPlotBase;
	int m_nProjection;
	SINGLE m_snScaleAngle;
	CSeries m_oSeries;
	int m_nSort;
	SINGLE m_snStartingAngle;
	int m_nSubPlotLabelPosition;
	bool m_bUniformAxis;
	CView3D m_oView3D;
	CWall m_oWall;
	SINGLE m_snWidthToHeightRatio;
	CWeighting m_oWeighting;
	SINGLE m_snxGap;
	CXYZ m_oXYZ;
	SINGLE m_snzGap;
public:
	CPlot();
};
}}

#endif // PLOT_H
