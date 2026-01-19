#ifndef SURFACE_H
#define SURFACE_H

#include "Brush.h"
#include "Pen.h"

namespace HWP { namespace CHART
{
class CSurface : public IChartObject
{
	CHART_INTEGER m_nBase;
	CBrush m_oBrush;
	CHART_INTEGER m_nColWireframe;
	CHART_INTEGER m_nDisplayType;
	CHART_INTEGER m_nProjection;
	CHART_INTEGER m_nRowWireframe;
	CPen m_oWireframePen;
public:
	CSurface();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // SURFACE_H
