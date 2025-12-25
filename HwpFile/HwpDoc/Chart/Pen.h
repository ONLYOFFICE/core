#ifndef PEN_H
#define PEN_H

#include "VtColor.h"

namespace HWP { namespace CHART
{
class CPen : public IChartObject
{
	int m_nCap;
	int m_nJoin;
	SINGLE m_snLimit;
	int m_nStyle;
	SINGLE m_snWidth;
	CVtColor m_oVtColor;
public:
	CPen();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // PEN_H
