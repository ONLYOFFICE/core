#ifndef ATTRIBUTE_H
#define ATTRIBUTE_H

#include "Brush.h"
#include "Pen.h"
#include "Collection.h"

namespace HWP { namespace CHART
{
class CAttribute : public IChartObject
{
	CBrush m_oBrush;
	CPen m_oPen;
	CHART_STRING m_sText;
	CHART_DOUBLE m_dValue;
public:
	CAttribute();

	bool Read(CChartStream& oStream) override;
};

using CAttributes = CCollection<CAttribute>;
}}

#endif // ATTRIBUTE_H
