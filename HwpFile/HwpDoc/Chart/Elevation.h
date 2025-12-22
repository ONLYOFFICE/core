#ifndef ELEVATION_H
#define ELEVATION_H

#include "Attribute.h"
#include "Contour.h"
#include "ContourGradient.h"
#include "Surface.h"

namespace HWP { namespace CHART
{
class CElevation
{
	CAttributes m_oAttributes;
	bool m_bAutoValues;
	int m_nColorType;
	int m_nColSmoothing;
	CContour m_oContour;
	CContourGradient m_oContourGradient;
	int m_nRowSmoothing;
	bool m_bSeparateContourData;
	CSurface m_oSurface;
public:
	CElevation();
};
}}

#endif // ELEVATION_H
