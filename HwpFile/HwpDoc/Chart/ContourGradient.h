#ifndef CONTOURGRADIENT_H
#define CONTOURGRADIENT_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CContourGradient : public IChartObject
{
	CVtColor m_oFromBrushColor;
	CVtColor m_oToBrushColor;
	CVtColor m_oFromPenColor;
	CVtColor m_oToPenColor;
public:
	CContourGradient();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // CONTOURGRADIENT_H
