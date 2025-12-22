#ifndef STATLINE_H
#define STATLINE_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CStatLine
{
	int m_nFlags;
	int m_nStyle;
	CVtColor m_oVtColor;
	SINGLE m_snWidth;
public:
	CStatLine();
};
}}

#endif // STATLINE_H
