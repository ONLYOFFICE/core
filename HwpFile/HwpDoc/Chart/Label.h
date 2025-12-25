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
	bool m_bAuto;
	CBackdrop m_oBackdrop;
	HWP_STRING m_sFormat;
	HWP_STRING m_sFormatLength;
	bool m_bStanding;
	CTextLayout m_oTextLayout;
	CVtFont m_oVtFont;
public:
	CLabel();

	bool Read(CHWPStream& oStream) override;
};

using CLabels = CCollection<CLabel>;
}}

#endif // LABEL_H
