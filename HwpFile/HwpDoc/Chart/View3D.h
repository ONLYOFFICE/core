#ifndef VIEW3D_H
#define VIEW3D_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CView3D : public IChartObject
{
	CHART_SINGLE m_snElevation;
	CHART_SINGLE m_snRotation;
public:
	CView3D();

	bool Read(CChartStream& oStream) override;
};
}}
#endif // VIEW3D_H
