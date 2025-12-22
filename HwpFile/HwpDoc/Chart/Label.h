#ifndef LABEL_H
#define LABEL_H

#include "Backdrop.h"
#include "TextLayout.h"
#include "VtFont.h"

namespace HWP { namespace CHART
{
class CLabel
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
};

using CLabels = ICollection<CLabel>;
}}

#endif // LABEL_H
