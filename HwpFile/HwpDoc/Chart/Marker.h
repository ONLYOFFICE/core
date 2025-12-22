#ifndef MARKER_H
#define MARKER_H

#include "Pen.h"
#include "VtPicture.h"

namespace HWP { namespace CHART
{
class CMarker
{
	CVtColor m_oFillColor;
	CPen m_oPen;
	SINGLE m_snSize;
	int m_nStyle;
	bool m_bVisible;
	CVtPicture m_oVtPicture;
public:
	CMarker();
};
}}

#endif // MARKER_H
