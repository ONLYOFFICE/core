#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Brush.h"
#include "Pen.h"

namespace HWP { namespace CHART
{
class CAttribute
{
	CBrush m_oBrush;
	CPen m_oPen;
	HWP_STRING m_sText;
	double m_dValue;
public:
	CAttribute();
};

using CAttributes = ICollection<CAttribute>;
}}

#endif // ATTRIBUTE_H
