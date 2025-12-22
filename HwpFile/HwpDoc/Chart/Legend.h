#ifndef LEGEND_H
#define LEGEND_H

#include "Backdrop.h"
#include "Location.h"
#include "TextLayout.h"
#include "VtFont.h"

namespace HWP { namespace CHART
{
class CLegend
{
	CBackdrop m_oBackdrop;
	CLocation m_oLocation;
	CTextLayout m_oTextLayout;
	CVtFont m_oVtFont;
public:
	CLegend();
};
}}

#endif // LEGEND_H
