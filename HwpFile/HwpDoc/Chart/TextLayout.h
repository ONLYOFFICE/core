#ifndef TEXTLAYOUT_H
#define TEXTLAYOUT_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CTextLayout : public IChartObject
{
	CHART_BOOLEAN m_bWordWrap;
	CHART_INTEGER m_nHorzAlignment;
	CHART_INTEGER m_nOrientation;
	CHART_INTEGER m_nVertAlignment;
public:
	CTextLayout();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // TEXTLAYOUT_H
