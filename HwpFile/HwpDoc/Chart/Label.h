#ifndef LABEL_H
#define LABEL_H

#include "Backdrop.h"
#include "TextLayout.h"
#include "VtFont.h"
#include "Collection.h"

namespace HWP { namespace CHART
{
class CLabel : public IChartObject
{
	CHART_BOOLEAN m_bAuto;
	CBackdrop m_oBackdrop;
	CHART_STRING m_sFormat;
	CHART_STRING m_sFormatLength;
	CHART_BOOLEAN m_bStanding;
	CTextLayout m_oTextLayout;
	CVtFont m_oVtFont;
public:
	CLabel();

	bool Read(CChartStream& oStream) override;
};

using CLabels = CCollection<CLabel>;
}}

#endif // LABEL_H
