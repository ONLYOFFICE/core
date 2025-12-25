#ifndef CONTOUR_H
#define CONTOUR_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CContour : public IChartObject
{
	int m_nDisplayType;
public:
	CContour();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // CONTOUR_H
