#ifndef TITLE_H
#define TITLE_H

#include "Backdrop.h"
#include "Location.h"
#include "TextLayout.h"
#include "VtFont.h"

namespace HWP { namespace CHART
{
class CTitle
{
	CBackdrop m_oBackdrop;
	CLocation m_oLocation;
	HWP_STRING m_sText;
	CTextLayout m_oTextLayout;
	int m_nTextLength;
	CVtFont m_oVtFont;
public:
	CTitle();
};
}}

#endif // TITLE_H
