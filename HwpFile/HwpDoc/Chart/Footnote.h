#ifndef FOOTNOTE_H
#define FOOTNOTE_H

#include "Backdrop.h"
#include "Location.h"
#include "TextLayout.h"
#include "VtFont.h"

namespace HWP { namespace CHART
{
class CFootnote : public IChartObject
{
	CBackdrop m_oBackdrop;
	CLocation m_oLocation;
	CHART_STRING m_sText;
	CTextLayout m_oTextLayout;
	CHART_INTEGER m_nTextLength;
	CVtFont m_oVtFont;
public:
	CFootnote();

	bool Read(CChartStream& oStream) override;
};
}}

#endif // FOOTNOTE_H
