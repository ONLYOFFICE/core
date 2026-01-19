#ifndef VTFONT_H
#define VTFONT_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CVtFont : public IChartObject
{
	CVtColor m_oColor;
	CHART_INTEGER m_nEffects;
	CHART_STRING m_sName;
	CHART_SINGLE m_snSize;
	CHART_INTEGER m_nStyle;
public:
	CVtFont();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // VTFONT_H
