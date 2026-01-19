#ifndef MARKER_H
#define MARKER_H

#include "Pen.h"
#include "VtPicture.h"

namespace HWP { namespace CHART
{
class CMarker : public IChartObject
{
	CVtColor m_oFillColor;
	CPen m_oPen;
	CHART_SINGLE m_snSize;
	CHART_INTEGER m_nStyle;
	CHART_BOOLEAN m_bVisible;
	CVtPicture m_oVtPicture;
public:
	CMarker();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // MARKER_H
