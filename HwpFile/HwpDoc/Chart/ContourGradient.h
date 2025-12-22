#ifndef CONTOURGRADIENT_H
#define CONTOURGRADIENT_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CContourGradient
{
	CVtColor m_oFromBrushColor;
	CVtColor m_oToBrushColor;
	CVtColor m_oFromPenColor;
	CVtColor m_oToPenColor;
public:
	CContourGradient();
};
}}

#endif // CONTOURGRADIENT_H
