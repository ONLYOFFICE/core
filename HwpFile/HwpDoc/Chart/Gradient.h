#ifndef GRADIENT_H
#define GRADIENT_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CGradient
{
	CVtColor m_oFromColor;
	int m_nStyle;
	CVtColor m_oToColor;
public:
	CGradient();
};
}}

#endif // GRADIENT_H
