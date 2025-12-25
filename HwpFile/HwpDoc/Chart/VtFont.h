#ifndef VTFONT_H
#define VTFONT_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CVtFont : public IChartObject
{
	CVtColor m_oColor;
	int m_nEffects;
	HWP_STRING m_sName;
	SINGLE m_snSize;
	int m_nStyle;
public:
	CVtFont();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // VTFONT_H
