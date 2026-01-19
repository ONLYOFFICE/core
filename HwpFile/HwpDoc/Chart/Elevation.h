#ifndef ELEVATION_H
#define ELEVATION_H

#include "Attribute.h"
#include "Contour.h"
#include "ContourGradient.h"
#include "Surface.h"

namespace HWP { namespace CHART
{
class CElevation : public IChartObject
{
	CAttributes m_oAttributes;
	CHART_BOOLEAN m_bAutoValues;
	CHART_INTEGER m_nColorType;
	CHART_INTEGER m_nColSmoothing;
	CContour m_oContour;
	CContourGradient m_oContourGradient;
	CHART_INTEGER m_nRowSmoothing;
	CHART_BOOLEAN m_bSeparateContourData;
	CSurface m_oSurface;
public:
	CElevation();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // ELEVATION_H
