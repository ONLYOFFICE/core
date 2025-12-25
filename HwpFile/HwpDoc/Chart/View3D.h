#ifndef VIEW3D_H
#define VIEW3D_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CView3D : public IChartObject
{
	SINGLE m_snElevation;
	SINGLE m_snRotation;
public:
	CView3D();

	bool Read(CHWPStream& oStream) override;
};
}}
#endif // VIEW3D_H
