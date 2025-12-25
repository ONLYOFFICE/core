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
	SINGLE m_snSize;
	int m_nStyle;
	bool m_bVisible;
	CVtPicture m_oVtPicture;
public:
	CMarker();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // MARKER_H
