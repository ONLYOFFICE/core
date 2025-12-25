#ifndef BRUSH_H
#define BRUSH_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CBrush : public IChartObject
{
	CVtColor m_oFillColor;
	int m_nIndex;
	CVtColor m_oPatternColor;
	int m_nStyle;
public:
	CBrush();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // BRUSH_H
