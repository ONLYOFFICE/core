#ifndef SURFACE_H
#define SURFACE_H

#include "Brush.h"
#include "Pen.h"

namespace HWP { namespace CHART
{
class CSurface : public IChartObject
{
	int m_nBase;
	CBrush m_oBrush;
	int m_nColWireframe;
	int m_nDisplayType;
	int m_nProjection;
	int m_nRowWireframe;
	CPen m_oWireframePen;
public:
	CSurface();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // SURFACE_H
