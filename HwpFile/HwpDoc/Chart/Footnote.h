#ifndef FOOTNOTE_H
#define FOOTNOTE_H

#include "Backdrop.h"
#include "Location.h"
#include "TextLayout.h"
#include "VtFont.h"

namespace HWP { namespace CHART
{
class CFootnote
{
	CBackdrop m_oBackdrop;
	CLocation m_oLocation;
	HWP_STRING m_sText;
	CTextLayout m_oTextLayout;
	int m_nTextLength;
	CVtFont m_oVtFont;
public:
	CFootnote();
};
}}

#endif // FOOTNOTE_H
