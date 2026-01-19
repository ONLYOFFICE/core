#ifndef LEGEND_H
#define LEGEND_H

#include "Backdrop.h"
#include "Location.h"
#include "TextLayout.h"
#include "VtFont.h"

namespace HWP { namespace CHART
{
class CLegend : public IChartObject
{
	CBackdrop m_oBackdrop;
	CLocation m_oLocation;
	CTextLayout m_oTextLayout;
	CVtFont m_oVtFont;
public:
	CLegend();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // LEGEND_H
