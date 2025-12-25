#ifndef CATEGORYSCALE_H
#define CATEGORYSCALE_H

#include "ChartObject.h"

namespace HWP { namespace CHART
{
class CCategoryScale : public IChartObject
{
	bool m_bAuto;
	int m_nDivisionsPerLabel;
	int m_nDivisionsPerTick;
	bool m_bLabelTick;
public:
	CCategoryScale();

	bool Read(CHWPStream& oStream) override;
};
}}

#endif // CATEGORYSCALE_H
