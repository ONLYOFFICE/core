#ifndef TITLE_H
#define TITLE_H

#include "Backdrop.h"
#include "Location.h"
#include "TextLayout.h"
#include "VtFont.h"

namespace HWP { namespace CHART
{
class CTitle : public IChartObject
{
	CBackdrop m_oBackdrop;
	CLocation m_oLocation;
	CHART_STRING m_sText;
	CTextLayout m_oTextLayout;
	CHART_INTEGER m_nTextLength;
	CVtFont m_oVtFont;
public:
	CTitle();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // TITLE_H
